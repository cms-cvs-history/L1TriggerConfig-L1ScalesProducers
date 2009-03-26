// -*- C++ -*-
//
// Package:    L1HfRingEtScaleOnlineProd
// Class:      L1HfRingEtScaleOnlineProd
// 
/**\class L1HfRingEtScaleOnlineProd L1HfRingEtScaleOnlineProd.h L1TriggerConfig/L1ScalesProducers/src/L1HfRingEtScaleOnlineProd.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Werner Man-Li Sun
//         Created:  Tue Sep 16 22:43:22 CEST 2008
// $Id: L1HfRingEtScaleOnlineProd.cc,v 1.1 2009/03/18 11:03:04 efron Exp $
//
//


// system include files

// user include files
#include "CondTools/L1Trigger/interface/L1ConfigOnlineProdBase.h"

#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/DataRecord/interface/L1HfRingEtScaleRcd.h"


//
// class declaration
//

class L1HfRingEtScaleOnlineProd :
  public L1ConfigOnlineProdBase< L1HfRingEtScaleRcd, L1CaloEtScale > {
   public:
      L1HfRingEtScaleOnlineProd(const edm::ParameterSet&);
      ~L1HfRingEtScaleOnlineProd();

  virtual boost::shared_ptr< L1CaloEtScale > newObject(
    const std::string& objectKey ) ;


   private:
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1HfRingEtScaleOnlineProd::L1HfRingEtScaleOnlineProd(
  const edm::ParameterSet& iConfig)
  : L1ConfigOnlineProdBase< L1HfRingEtScaleRcd, L1CaloEtScale >( iConfig )
{
   //the following line is needed to tell the framework what
   // data is being produced

   //now do what ever other initialization is needed
}


L1HfRingEtScaleOnlineProd::~L1HfRingEtScaleOnlineProd()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

boost::shared_ptr< L1CaloEtScale >
L1HfRingEtScaleOnlineProd::newObject( const std::string& objectKey )
{
     using namespace edm::es;

     // get thresholds
     std::vector< std::string > queryStrings ;
     queryStrings.push_back( "E_GEV_BIN_LOW_0");
     queryStrings.push_back( "E_GEV_BIN_LOW_1");
     queryStrings.push_back( "E_GEV_BIN_LOW_2");
     queryStrings.push_back( "E_GEV_BIN_LOW_3");
     queryStrings.push_back( "E_GEV_BIN_LOW_4");
     queryStrings.push_back( "E_GEV_BIN_LOW_5");
     queryStrings.push_back( "E_GEV_BIN_LOW_6");
     queryStrings.push_back( "E_GEV_BIN_LOW_7");


     const l1t::OMDSReader::QueryResults scaleKeyResults =
       m_omdsReader.singleAttribute( objectKey ) ;
     
     l1t::OMDSReader::QueryResults scaleResults =
       m_omdsReader.basicQuery( queryStrings,
                                "CMS_GT",
                                "L1T_SCALE_CALO_HF_ET_SUM",
                                "L1T_SCALE_CALO_HF_ET_SUM.ID",
				scaleKeyResults
				);

     if( scaleResults.queryFailed() ||
	 scaleResults.numberRows() != 1 ) // check query successful
       {
	 edm::LogError( "L1-O2O" ) << "Problem with L1HfRingEtScale key." ;
	 return boost::shared_ptr< L1CaloEtScale >() ;
       }

     std::vector<double> thresholds;
     for( std::vector< std::string >::iterator thresh = queryStrings.begin();
	  thresh != queryStrings.end(); ++thresh) {
       float tempScale;
       scaleResults.fillVariable(*thresh,tempScale);
       thresholds.push_back(tempScale);
     }

     // get region LSB
     l1t::OMDSReader::QueryResults lsbResults =
       m_omdsReader.basicQuery( "GCT_RGN_ET_LSB",
			      "CMS_GCT",
			      "GCT_PHYS_PARAMS",
			      "GCT_PHYS_PARAMS.CONFIG_KEY",
			      m_omdsReader.singleAttribute( objectKey ) ) ;
     
     if( lsbResults.queryFailed() ) // check if query was successful
       {
	 edm::LogError( "L1-O2O" ) << "Problem with L1GctJetEtScale key." ;
	 return boost::shared_ptr< L1CaloEtScale >() ;
       }
     
     double rgnEtLsb=0.;
     lsbResults.fillVariable( "GCT_RGN_ET_LSB", rgnEtLsb );


     //~~~~~~~~~ Instantiate new L1HfRingEtScale object. ~~~~~~~~~
     return boost::shared_ptr< L1CaloEtScale >( new L1CaloEtScale(0xff, 0x7, rgnEtLsb, thresholds ) );
}


// ------------ method called to produce the data  ------------


//define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(L1HfRingEtScaleOnlineProd);