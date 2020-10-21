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

#include "ParticleData/PDGCodes.h"
#include "EventGen/EventRecord.h"
#include "GHEP/GHepParticle.h"
#include "Ntuple/NtpMCEventRecord.h"

extern char *optarg;
extern int optind, opterr, optopt;
std::string input_filename;
std::string output_filename;
int num_events;
void parse_command_line(int argc, char** argv);

int main(int argc, char ** argv)
{

  parse_command_line(argc, argv);

  //TFile file(input_filename.c_str(),"READ");
  TFile *file = TFile::Open(input_filename.c_str());
  TTree *tree = 0;
  //TTree *tree = dynamic_cast <TTree*> (file.Get("gtree"));
  file->GetObject("gtree",tree);
  //TTree *tree = dynamic_cast <TTree*> (file.Get(gtree));
  genie::NtpMCEventRecord *eventbranch;
  tree->SetBranchAddress("gmcrec", &eventbranch);

  if (num_events <= 0){
    num_events = (int) tree->GetEntries();
  }

  std::cout <<"num_events: "<<num_events<<std::endl;

  // set up output file
  std::ofstream outfile(output_filename.c_str());

  // Loop over all events
  double time = 0;
  for(Long64_t i = 0; i < num_events; i++) {
    
    //Mark the beginning of a new event
    outfile << "<<<<UTC    ID    InteractionName>>>>" << std::endl;

    std::cout <<"Getting entry "<<i<<std::endl;
    // get next tree entry
    tree->GetEntry(i);

    std::cout <<"Getting event, interaction and proc"<<std::endl;
    // get the GENIE event
    //genie::EventRecord &event = *(eventbranch->event);
    //std::cout << event << std::endl;
    genie::Interaction *interaction = eventbranch->event->Summary(); // loads interaction type (CC, NC, RES, etc..)
    const genie::ProcessInfo & proc = interaction->ProcInfo();
    
    double x = 1000.0*eventbranch->event->Vertex()->X(); // distances from GENIE are in meters
    double y = 1000.0*eventbranch->event->Vertex()->Y(); // distances from GENIE are in meters
    double z = 1000.0*eventbranch->event->Vertex()->Z(); // distances from GENIE are in meters

    double utc = time;
    //TTimeStamp utc(time,0);
    // mc->SetUTC(utc); // GENIE does not give global time, so we just add arbitrary times for RAT 
    //mc->SetID(i);
    //mcsummary->SetInteractionName(TString(proc.InteractionTypeAsString()) + TString(proc.ScatteringTypeAsString()));
     
    outfile << utc << "    " << i << "    " << TString(proc.InteractionTypeAsString()) + TString(proc.ScatteringTypeAsString()) << std::endl;
    //outfile << utc << "    " << i << "    " << "SomeInteraction" << std::endl;

    outfile << "<<<<ParticleType    PDG    Time    PositionX    PositionY    PositionZ    MomentumX    MomentumY    MomentumZ    KE>>>>"<<std::endl;
    // Loop over all particles in this event

    //TIter event_iter(&event);

    int nparticles = eventbranch->event->GetEntries();
    //while((genieparticle=dynamic_cast<genie::GHepParticle *>(event_iter.Next())))
    for (int ipart=0; ipart < nparticles; ipart++)
    {
     
      genie::GHepParticle *genieparticle = eventbranch->event->Particle(ipart);
      // check if it is the initial state neutrino
      if (genieparticle->Status() == genie::kIStInitialState){ 
        outfile <<"Parent" << "    " << genieparticle->Pdg() << "    " << 0 << "    " << x << "    " << y << "    " << z << "    " << 1000.0*genieparticle->Px() << "    " << 1000.0*genieparticle->Py() << "    " << 1000.0*genieparticle->Pz() << "    " << 1000.0*genieparticle->KinE() << std::endl;
      }
      if (genieparticle->Status() == genie::kIStStableFinalState){
        // skip if it is a GENIE special particle (final state unsimulated hadronic energy etc)
        if (genieparticle->Pdg() > 2000000000){
          continue;
        }
        outfile <<"Particle" << "    " << genieparticle->Pdg() << "    " << 0 << "    " << x << "    " << y << "    " << z << "    " << 1000.0*genieparticle->Px() << "    " << 1000.0*genieparticle->Py() << "    " << 1000.0*genieparticle->Pz() << "    " << 1000.0*genieparticle->KinE() << std::endl;
      }
    }// end loop over particles 

    eventbranch->Clear();

    time += 1; // GENIE does not calculate time of events so we arbitrarily add a second
  }//end loop over events

  // close input GHEP event file
  file->Close();
  outfile.close();

  // Delete ROOT-file and associated objects
  delete file;

  return 0;
}

void parse_command_line(int argc, char **argv)
{
  input_filename = "gntp.1.ghep.root";
  output_filename = "output.txt";
  num_events = -1;

  static struct option opts[] = { {"input", 1, NULL, 'i'},
                                  {"output", 1, NULL, 'o'},
                                  {"num-events", 1, NULL, 'N'} };
  int option_index = 0;
  int c = getopt_long(argc, argv, "i:o:N", opts, &option_index);
  while (c != -1) {
        switch (c) {
          case 'i': input_filename = optarg; break;
          case 'o': output_filename = optarg; break;
          case 'N': num_events = atol(optarg); break;
          default: exit(1);
        }
        c = getopt_long(argc, argv, "i:o:N", opts, &option_index);
  }
}
