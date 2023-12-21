#include <iostream>

#include "tradingsystem/BondAlgoExecution/BondAlgoExecutionService.hpp"
#include "tradingsystem/BondAlgoExecution/BondAlgoExecutionServiceListener.hpp"
#include "tradingsystem/BondAlgoStreaming/BondAlgoStreamingService.hpp"
#include "tradingsystem/BondAlgoStreaming/BondAlgoStreamingServiceListener.hpp"
#include "tradingsystem/BondExecution/BondExecutionService.hpp"
#include "tradingsystem/BondExecution/BondExecutionServiceConnector.hpp"
#include "tradingsystem/BondExecution/BondExecutionServiceListener.hpp"
#include "tradingsystem/BondGUI/BondGUIService.hpp"
#include "tradingsystem/BondGUI/BondGUIServiceConnector.hpp"
#include "tradingsystem/BondGUI/BondGUIServiceListener.hpp"
#include "tradingsystem/BondInquiry/BondInquiryService.hpp"
#include "tradingsystem/BondMarketData/BondMarketDataService.hpp"
#include "tradingsystem/BondMarketData/BondMarketDataServiceConnector.hpp"
#include "tradingsystem/BondPosition/BondPositionService.hpp"
#include "tradingsystem/BondPosition/BondPositionServiceListener.hpp"
#include "tradingsystem/BondPricing/BondPricingService.hpp"
#include "tradingsystem/BondPricing/BondPricingServiceConnector.hpp"
#include "tradingsystem/BondRisk/BondRiskService.hpp"
#include "tradingsystem/BondRisk/BondRiskServiceListener.hpp"
#include "tradingsystem/BondStreaming/BondStreamingService.hpp"
#include "tradingsystem/BondStreaming/BondStreamingServiceConnector.hpp"
#include "tradingsystem/BondStreaming/BondStreamingServiceListener.hpp"
#include "tradingsystem/BondTrading/BondTradeBookingService.hpp"
#include "tradingsystem/BondTrading/BondTradeBookingServiceConnector.hpp"
#include "tradingsystem/BondTrading/BondTradeBookingServiceListener.hpp"
#include "tradingsystem/BondData/BondHistoricalDataService.hpp"
#include "tradingsystem/BondData/BondHistoricalDataServiceConnector.hpp"
#include "tradingsystem/BondData/BondHistoricalDataServiceListener.hpp"
#include "tradingsystem/executionservice.hpp"
#include "tradingsystem/bonds.hpp"
#include "tradingsystem/guiservice.hpp"
#include "tradingsystem/historicaldataservice.hpp"
#include "tradingsystem/inquiryservice.hpp"
#include "tradingsystem/marketdataservice.hpp"
#include "tradingsystem/positionservice.hpp"
#include "tradingsystem/pricingservice.hpp"
#include "tradingsystem/products.hpp"
#include "tradingsystem/riskservice.hpp"
#include "tradingsystem/soa.hpp"
#include "tradingsystem/streamingservice.hpp"
#include "tradingsystem/tradebookingservice.hpp"
#include "tradingsystem/utility.hpp"

#include <map>
#include <vector>
#include <string>

#include "boost/date_time/posix_time/posix_time.hpp"
using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::posix_time::microsec_clock;

//CUSIP     Maturity    Coupon  PV01
//91282CFX4�11/30/2024��4.500 � 0.01879
//91282CFW6�11/15/2025 �4.500�� 0.02761
//91282CFZ9�11/30/2027 �3.875�� 0.04526
//91282CFY2�11/30/2029 �3.875 � 0.06170
//91282CFV8�11/15/2032��4.125 � 0.08598
//912810TM0�11/15/2042 �4.000�� 0.14420
//912810TL2�11/15/2052� 4.000   0.19917


int main()
{
	//// STEP 1: CREATE ALL SERVICES, CONNECTORS, AND LISTENERS ******************************************************************************
	BondAlgoExecutionService bondAlgoExecutionService;
	BondAlgoExecutionServiceListener bondAlgoExecutionServiceListener(&bondAlgoExecutionService);

	BondAlgoStreamingService bondAlgoStreamingService;
	BondAlgoStreamingServiceListener bondAlgoStreamingServiceListener(&bondAlgoStreamingService);

	BondExecutionService bondExecutionService;
	BondExecutionServiceConnector bondExecutionServiceConnector = bondExecutionService.GetConnector();
	BondExecutionServiceListener bondExecutionServiceListener(&bondExecutionService);

	BondGUIService bondGUIService;
	BondGUIServiceConnector bondGUIServiceConnector = bondGUIService.GetConnector();
	BondGUIServiceListener bondGUIServiceListener(&bondGUIService);

	BondInquiryService bondInquiryService;
	BondInquiryServiceConnector bondInquiryServiceConnector = bondInquiryService.GetConnector();

	BondMarketDataService bondMarketDataService;
	BondMarketDataServiceConnector bondMarketDataServiceConnector(&bondMarketDataService);

	BondPositionService bondPositionService;
	BondPositionServiceListener bondPositionServiceListener(&bondPositionService);

	BondPricingService bondPricingService;
	BondPricingServiceConnector bondPricingServiceConnector(&bondPricingService);

	BondRiskService bondRiskService;
	BondRiskServiceListener bondRiskServiceListener(&bondRiskService);

	BondStreamingService bondStreamingService;
	BondStreamingServiceConnector bondStreamingServiceConnector = bondStreamingService.GetConnector();
	BondStreamingServiceListener bondStreamingServiceListener(&bondStreamingService);

	BondTradeBookingService bondTradeBookingService;
	BondTradeBookingServiceConnector bondTradeBookingServiceConnector(&bondTradeBookingService);
	BondTradeBookingServiceListener bondTradeBookingServiceListener(&bondTradeBookingService);

	// the following HistoricalDataService classes are generic
	PositionHistoricalDataService<Bond> bondPositionHistoricalDataService;
	PositionHistoricalDataServiceConnector<Bond> bondPositionHistoricalDataServiceConnector = bondPositionHistoricalDataService.GetConnector();
	PositionHistoricalDataServiceListener<Bond> bondPositionHistoricalDataServiceListener(&bondPositionHistoricalDataService);

	RiskHistoricalDataService<Bond> bondRiskHistoricalDataService;
	RiskHistoricalDataServiceConnector<Bond> bondRiskHistoricalDataServiceConnector = bondRiskHistoricalDataService.GetConnector();
	RiskHistoricalDataServiceListener<Bond> bondRiskHistoricalDataServiceListener(&bondRiskHistoricalDataService);

	std::vector<Bond> frontEndBonds = { CUSIP_to_BOND["91282CFX4"], CUSIP_to_BOND["91282CFW6"] };	// 2Y, 3Y
	std::vector<Bond> bellyBonds = { CUSIP_to_BOND["91282CFZ9"], CUSIP_to_BOND["91282CFY2"], CUSIP_to_BOND["91282CFV8"] };	// 5Y, 7Y, 10Y
	std::vector<Bond> longEndBonds = { CUSIP_to_BOND["912810TM0"], CUSIP_to_BOND["912810TL2"] };	// 20Y, 30Y

	BucketedSector<Bond> frontEnd(frontEndBonds, "FrontEnd");
	BucketedSector<Bond> belly(bellyBonds, "Belly");
	BucketedSector<Bond> longEnd(longEndBonds, "LongEnd");
    
	bondRiskHistoricalDataService.AddBucketedSector(frontEnd);
	bondRiskHistoricalDataService.AddBucketedSector(belly);
	bondRiskHistoricalDataService.AddBucketedSector(longEnd);


	ExecutionHistoricalDataService<Bond> bondExecutionHistoricalDataService;
	ExecutionHistoricalDataServiceConnector<Bond> bondExecutionHistoricalDataServiceConnector = bondExecutionHistoricalDataService.GetConnector();
	ExecutionHistoricalDataServiceListener<Bond> bondExecutionHistoricalDataServiceListener(&bondExecutionHistoricalDataService);

	StreamingHistoricalDataService<Bond> bondStreamingHistoricalDataService;
	StreamingHistoricalDataServiceConnector<Bond> bondStreamingHistoricalDataServiceConnector = bondStreamingHistoricalDataService.GetConnector();
	StreamingHistoricalDataServiceListener<Bond> bondStreamingHistoricalDataServiceListener(&bondStreamingHistoricalDataService);

	InquiryHistoricalDataService<Bond> bondInquiryHistoricalDataService;
	InquiryHistoricalDataServiceConnector<Bond> bondInquiryHistoricalDataServiceConnector = bondInquiryHistoricalDataService.GetConnector();
	InquiryHistoricalDataServiceListener<Bond> bondInquiryHistoricalDataServiceListener(&bondInquiryHistoricalDataService);

	////STEP 2: REGISTER THE LISTENERS ON THE CORRESPONDING SERVICES ******************************************************************************
	// in the order of the instructions
	bondTradeBookingService.AddListener(&bondPositionServiceListener);//
	bondPositionService.AddListener(&bondRiskServiceListener);//
	bondMarketDataService.AddListener(&bondAlgoExecutionServiceListener);//
	bondAlgoExecutionService.AddListener(&bondExecutionServiceListener);//
	bondPricingService.AddListener(&bondAlgoStreamingServiceListener);//
	bondAlgoStreamingService.AddListener(&bondStreamingServiceListener);//
	bondPricingService.AddListener(&bondGUIServiceListener);// // only call bondGUIService.OnMessage() 100 times
	bondExecutionService.AddListener(&bondTradeBookingServiceListener);//
	bondPositionService.AddListener(&bondPositionHistoricalDataServiceListener);//
	bondRiskService.AddListener(&bondRiskHistoricalDataServiceListener);//
	bondExecutionService.AddListener(&bondExecutionHistoricalDataServiceListener);//
	bondStreamingService.AddListener(&bondStreamingHistoricalDataServiceListener);//
	bondInquiryService.AddListener(&bondInquiryHistoricalDataServiceListener);//
	// output files of this program:
	// gui.txt, positions.txt, risk.txt, executions.txt, streaming.txt, allinquiries.txt



	////// STEP3: LET THE CONNECTORS SUBSCRIBE TO .TXT FILES ******************************************************************************
	// in order of the instructions
	ptime t0 = microsec_clock::local_time();
	bondPricingServiceConnector.Subscribe("prices.txt");
	ptime t1 = microsec_clock::local_time();
	bondTradeBookingServiceConnector.Subscribe("trades.txt");
	ptime t2 = microsec_clock::local_time();
	bondMarketDataServiceConnector.Subscribe("marketdata.txt");
	ptime t3 = microsec_clock::local_time();
	bondInquiryServiceConnector.Subscribe("inquiries.txt");
	ptime t4 = microsec_clock::local_time();

	std::cout << "Time used for BondPricingServiceConnector.Subscribe(): " << t1 - t0 << std::endl;
	std::cout << "Time used for BondTradeBookingServiceConnector.Subscribe(): " << t2 - t1 << std::endl;
	std::cout << "Time used for BondMarketDataServiceConnector.Subscribe(): " << t3 - t2 << std::endl;
	std::cout << "Time used for BondInquiryServiceConnector.Subscribe(): " << t4 - t3 << std::endl;
	std::cout << "Total time used: " << t4 - t0 << std::endl;


	std::cout << "Program finished. Press Enter to exit..." << std::endl;
	std::cin.get();	// pause the program after it finishes

	return 0;
}
