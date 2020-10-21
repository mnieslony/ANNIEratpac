// takes the output of gevgen_atmo and creates a RAT root file
//
// genie2txt -i [input genie filename] -o [output root filename] (-N [number of events to process])

#include <unistd.h>
#include <getopt.h>
#include <string>
#include <fstream>

#include <TFile.h>
#include <TTree.h>
#include <TIterator.h>
#include <TVector3.h>

int num_events;

void genie2txt(const char* input_filename, const char* output_filename)
{

  genie::PDGLibrary::Instance();

  TFile *file = TFile::Open(input_filename,"READ");
  TTree *tree = 0;
  file->GetObject("gtree",tree);
  genie::NtpMCEventRecord *eventbranch = new genie::NtpMCEventRecord();
  tree->SetBranchAddress("gmcrec", &eventbranch);


  if (num_events <= 0){
    num_events = (int) tree->GetEntries();
  }

  std::cout <<"num_events: "<<num_events<<std::endl;

  // Set up output file
  std::ofstream outfile(output_filename);

  // Loop over all events
  double time = 0;
  for(int i = 0; i < num_events; i++) {
    
    // Mark the next event entry in the output file
    outfile << "<<<<UTC    ID    InteractionName>>>>" << std::endl;

    //std::cout <<"Getting entry "<<i<<std::endl;
    // Get next tree entry
    tree->GetEntry(i);

    //std::cout <<"Getting event, interaction and proc"<<std::endl;
    // Get the GENIE event
    genie::Interaction *interaction = eventbranch->event->Summary(); // loads interaction type (CC, NC, RES, etc..)
    const genie::ProcessInfo & proc = interaction->ProcInfo();
    
    double x = 1000.0*eventbranch->event->Vertex()->X(); // distances from GENIE are in meters
    double y = 1000.0*eventbranch->event->Vertex()->Y(); // distances from GENIE are in meters
    double z = 1000.0*eventbranch->event->Vertex()->Z(); // distances from GENIE are in meters

    double utc = time;
    
    outfile << utc << "    " << i << "    " << TString(proc.InteractionTypeAsString()) + TString(proc.ScatteringTypeAsString()) << std::endl;
    //outfile << utc << "    " << i << "    " << "SomeInteraction" << std::endl;

    outfile << "<<<<ParticleType    PDG    Time    PositionX    PositionY    PositionZ    MomentumX    MomentumY    MomentumZ    KE>>>>"<<std::endl;
    // Loop over all particles in this event

    int nparticles = eventbranch->event->GetEntries();

    for (int ipart=0; ipart < nparticles; ipart++)
    {
      genie::GHepParticle *genieparticle = eventbranch->event->Particle(ipart);
      // Check if it is the initial state neutrino
      if (genieparticle->Status() == genie::kIStInitialState){ 
        outfile <<"Parent" << "    " << genieparticle->Pdg() << "    " << 0 << "    " << x << "    " << y << "    " << z << "    " << 1000.0*genieparticle->Px() << "    " << 1000.0*genieparticle->Py() << "    " << 1000.0*genieparticle->Pz() << "    " << 1000.0*genieparticle->KinE() << std::endl;
      }
      if (genieparticle->Status() == genie::kIStStableFinalState){
        // Skip if it is a GENIE special particle (final state unsimulated hadronic energy etc)
        if (genieparticle->Pdg() > 2000000000){
          continue;
        }
        outfile <<"Particle" << "    " << genieparticle->Pdg() << "    " << 0 << "    " << x << "    " << y << "    " << z << "    " << 1000.0*genieparticle->Px() << "    " << 1000.0*genieparticle->Py() << "    " << 1000.0*genieparticle->Pz() << "    " << 1000.0*genieparticle->KinE() << std::endl;
      }
    }// End loop over particles 

    eventbranch->Clear();
    time += 1; // GENIE does not calculate time of events so we arbitrarily add a second
  }//end loop over events

  // close input GHEP event file
  file->Close();
  outfile.close();

}

