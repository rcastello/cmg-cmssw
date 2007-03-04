/**
 * \file EcalDigiDumperModule.h 
 * dummy module  for the test of  DaqFileInputService
 *   
 * 
 * $Date: 2006/10/26 23:35:39 $
 * $Revision: 1.10 $
 * \author N. Amapane - S. Argiro'
 * \author G. Franzoni
 *
 */

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>
#include <DataFormats/EcalDetId/interface/EcalDetIdCollections.h>

#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h>
#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h>

#include <iostream>
#include <vector>


using namespace cms;
using namespace std;


class EcalDigiDumperModule: public edm::EDAnalyzer{
  
 public:
  EcalDigiDumperModule(const edm::ParameterSet& ps){  
    verbosity      = ps.getUntrackedParameter<int>("verbosity",1);
    numChannel     = ps.getUntrackedParameter<int>("numChannel",10);
    numPN          = ps.getUntrackedParameter<int>("numPN",2);
    // ecal slot_min and slot_max to be added once EcalDCCHeaderBlock has been updated accordingly
  }

  
 protected:
  int verbosity;
  int numChannel;
  int numPN;

  void analyze( const edm::Event & e, const  edm::EventSetup& c){

    
    // retrieving crystal data from Event
    edm::Handle<EBDigiCollection>  digis;
    e.getByLabel("ecalEBunpacker", digis);

    // retrieving crystal PN diodes from Event
    edm::Handle<EcalPnDiodeDigiCollection>  PNs;
    e.getByLabel("ecalEBunpacker", PNs);

    // retrieve collection of errors in the mem gain data
    edm::Handle<EcalElectronicsIdCollection> gainMem;
    e.getByLabel("ecalEBunpacker", "EcalIntegrityMemChIdErrors", gainMem);
    
    // retrieve collection of errors in the mem gain data
    edm::Handle<EcalElectronicsIdCollection> MemId;
    e.getByLabel("ecalEBunpacker", "EcalIntegrityMemTtIdErrors", MemId);

    
    cout << "\n\n";

    if(gainMem->size()) {  
      cout << "\n\n^^^^^^^^^^^^^^^^^^ [EcalDigiDumperModule]  Size of collection of mem gain errors is: " << gainMem->size() << endl;
      cout << "                                  [EcalDigiDumperModule]  dumping the bit gain errors\n"  << endl;
      for (EcalElectronicsIdCollection::const_iterator errItr= gainMem->begin();
	   errItr  != gainMem->end(); 
	   ++errItr ) {
	EcalElectronicsId  id = (*errItr);
	cout << "channel: dccNum= " << id.dccId() << "\t tower= " << id.towerId() << "\t channelNum= " << id.channelId()
	     << " has problems in the gain bits" << endl;
      }// end of loop on gain errors in the mem
    }// end if
    

        
    if(MemId->size()) {  
      cout << "\n\n^^^^^^^^^^^^^^^^^^ [EcalDigiDumperModule]  Size of collection of mem tt_block_id errors is: " << MemId->size() << endl;
      cout << "                                  [EcalDigiDumperModule]  dumping the mem tt_block_idb errors\n"  << endl;
      for (EcalElectronicsIdCollection::const_iterator errItr= MemId->begin();
	   errItr  != MemId->end(); 
	   ++errItr ) {
	EcalElectronicsId  id = (*errItr);
	cout << "tower_block: dccNum= " << id.dccId() << "\t tower= " << id.towerId() << " has ID problems " << endl;
      }// end of loop tower_block_id errors in the mem
    }// end if
    

    
    cout << "\n\n^^^^^^^^^^^^^^^^^^ [EcalDigiDumperModule]  digi cry collection size " << digis->size() << endl;
    cout << "                                  [EcalDigiDumperModule]  dumping first " << numChannel << " crystals\n";
    short dumpCounter =0;      

    if (verbosity>0)
      {
	for ( EBDigiCollection::const_iterator digiItr= digis->begin();digiItr != digis->end(); 
	      ++digiItr ) {
	  
	  {
	    if( (dumpCounter++) >= numChannel) break;
	    cout << "ic-cry: " 
		 << EBDetId((*digiItr).id()).ic() << " i-phi: " 
		 << EBDetId((*digiItr).id()).iphi() << " j-eta: " 
		 << EBDetId((*digiItr).id()).ieta();

	    for ( int i=0; i< (*digiItr).size() ; ++i ) {
	    if (!(i%5)  )  cout << "\n\t";
	    cout << "sId: " << (i+1) << " "
		 <<  (*digiItr).sample(i) << "\t";
	    }       
	    cout << " " << endl;
	  } 
	}
      }
    
    

    cout << "\n\n^^^^^^^^^^^^^^^^^^ EcalDigiDumperModule  digi PN collection.  Size: " << PNs->size() << endl;
    cout << "                                  [EcalDigiDumperModule]  dumping first " << numPN << " PNs ";
    dumpCounter=0;
    if (verbosity>0)
      {
	for ( EcalPnDiodeDigiCollection::const_iterator pnItr = PNs->begin(); pnItr != PNs->end(); ++pnItr ) {
	  
	  if( (dumpCounter++) >= numPN) break;

	  cout << "\nPN num: " 
	       << (*pnItr).id().iPnId();
	  
	  for ( int samId=0; samId < (*pnItr).size() ; samId++ ) {
	    if (!(samId%5)  )  cout << "\n\t";
	    cout <<  "sId: " << (samId+1) << " "
		 << (*pnItr).sample(samId) 
		 << "\t";
	  }//  PN samples
	  
	  
	}// PNs
      }
    
    

    // retrieving crystal TP from the Event
    edm::Handle<EcalTrigPrimDigiCollection>  primitives;
    e.getByLabel("ecalEBunpacker", primitives);
    
    cout << "\n\n^^^^^^^^^^^^^^^^^^ EcalDigiDumperModule  digi TP collection.  Size: " << primitives->size() << endl;
    cout << "                                  [EcalDigiDumperModule]  dumping primitives "  << endl;
    if (verbosity>0)
      {
	for ( EcalTrigPrimDigiCollection::const_iterator TPtr = primitives->begin();
	      ( TPtr != primitives->end()  && (TPtr-primitives->begin())<4 ); 
		++TPtr ) {
	  cout << "[EcalDigiDumperModule] tower: " << ( (TPtr-primitives->begin()) +1) 
	       << "\n" << (*TPtr) << endl;
	}
      }


 
  } // produce

};// class EcalDigiDumperModule

DEFINE_FWK_MODULE(EcalDigiDumperModule);
