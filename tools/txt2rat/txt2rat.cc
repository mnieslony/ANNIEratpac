// takes the output of gevgen_atmo and creates a RAT root file
//
// txt2rat -i [input genie filename] -o [output root filename] (-N [number of events to process])

#include <unistd.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TIterator.h>
#include <TVector3.h>

#include <RAT/DS/Root.hh>
#include <RAT/DS/MC.hh>
#include <RAT/DS/MCParticle.hh>

extern char *optarg;
extern int optind, opterr, optopt;
std::string input_filename;
std::string output_filename;
int num_events;
void parse_command_line(int argc, char** argv);

int main(int argc, char ** argv)
{

  parse_command_line(argc, argv);

  // Set up input file
  std::ifstream file(input_filename.c_str());

  // set up output file
  TFile *outfile = new TFile(output_filename.c_str(),"RECREATE");
  TTree *outtree = new TTree("T","RAT Tree");
  RAT::DS::Root *branchDS = new RAT::DS::Root();
  outtree->Branch("ds",branchDS->ClassName(),&branchDS,32000,99);

  // Loop over all events
  // Define all needed elements
  double time = 0;
  int id = 0;
  std::string interaction_name;
  std::string particle_type;
  int pdg = 0;
  double t = 0;
  double posx = 0;
  double posy = 0;
  double posz = 0;
  double momx = 0;
  double momy = 0;
  double momz = 0;
  double ke = 0;  
  std::string temp_string;

  RAT::DS::Root *ds = nullptr;
  RAT::DS::MC *mc = nullptr;
  RAT::DS::MCSummary *mcsummary = nullptr;

  while (!file.eof()){
    file >> temp_string;
    std::cout <<"temp_string: "<<temp_string<<std::endl;

    if (temp_string == "<<<<UTC"){
      file >> temp_string >> temp_string;	//Get ID & InteractionName>>>>
      file >> time >> id >> interaction_name;
      
      ds = new RAT::DS::Root();
      mc = ds->GetMC();
      mcsummary = mc->GetMCSummary();
      TTimeStamp utc(time,0);
      mc->SetUTC(utc); // GENIE does not give global time, so we just add arbitrary times for RAT 
      mc->SetID(id);
      mcsummary->SetInteractionName(TString(interaction_name));
      std::cout <<"Getting Event ID "<<id<<": Interaction = "<<interaction_name<<std::endl;
    }
    else if (temp_string == "ID"){
      file >> temp_string;	//Get InteractionName>>>>
      file >> time >> id >> interaction_name;
    
      ds = new RAT::DS::Root();
      mc = ds->GetMC();
      mcsummary = mc->GetMCSummary();
      TTimeStamp utc(time,0);
      mc->SetUTC(utc); // GENIE does not give global time, so we just add arbitrary times for RAT 
      mc->SetID(id);
      mcsummary->SetInteractionName(TString(interaction_name));
      std::cout <<"Getting Event ID "<<id<<": Interaction = "<<interaction_name<<std::endl;
    }
    else if (temp_string == "<<<<ParticleType"){
      file >> temp_string >> temp_string >> temp_string >> temp_string >> temp_string >> temp_string >> temp_string >> temp_string >> temp_string;
      //Check if next entry is particle line or next event
      while (file >> temp_string){
        std::cout <<"temp_string (loop): "<<temp_string<<std::endl;
        if (temp_string == "<<<<UTC") {
          *branchDS = *ds;
          outtree->Fill();
          break;
        } else particle_type = temp_string;
        file >> pdg >> t >> posx >> posy >> posz >> momx >> momy >> momz >> ke;
        if (particle_type == "Parent"){
          RAT::DS::MCParticle *ratparent = mc->AddNewMCParent();
          ratparent->SetPDGCode(pdg);
          ratparent->SetTime(0); // GENIE does not give particles time separate from event
          ratparent->SetPosition(TVector3(posx,posy,posz));
          ratparent->SetMomentum(TVector3(momx,momy,momz));
          ratparent->SetKE(ke);
          std::cout <<"Parsing parent particle with pdg "<<pdg<<", (X,Y,Z)  = ("<<posx<<","<<posy<<","<<posz<<"), (Px,Py,Pz) = ("<<momx<<","<<momy<<","<<momz<<"), Ekin = "<<ke<<std::endl;
        } else if (particle_type == "Particle"){
          RAT::DS::MCParticle *ratparticle = mc->AddNewMCParticle();
          ratparticle->SetPDGCode(pdg);
          ratparticle->SetTime(0); // GENIE does not give particles time separate from event
          ratparticle->SetPosition(TVector3(posx,posy,posz));
          ratparticle->SetMomentum(TVector3(momx,momy,momz)); // GENIE outputs momentum and energy in GeV
          ratparticle->SetKE(ke);
          std::cout <<"Parsing normal particle with pdg "<<pdg<<", (X,Y,Z)  = ("<<posx<<","<<posy<<","<<posz<<"), (Px,Py,Pz) = ("<<momx<<","<<momy<<","<<momz<<"), Ekin = "<<ke<<std::endl;
        }
      }
    }  
  }//end loop over events

  // close input GHEP event file
  file.close();
  outfile->Write();
  outfile->Close();

  return 0;
}

void parse_command_line(int argc, char **argv)
{
  input_filename = "gntp.1.txt";
  output_filename = "output.root";
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
