// TestFusionCrowd.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <direct.h>

#include "TestCases/ITestCase.h"

#include "TestCases/DebugTestCase.h"

#include "TestCases/TradeshowTestCase.h"
#include "TestCases/NeighbourSearchBenchCase.h"
#include "TestCases/CrossingTestCase.h"
#include "TestCases/PinholeTestCase.h"
#include "TestCases/TshapedFancyTestCase.h"
#include "TestCases/ExchangeCircleCase.h"
#include "TestCases/StenkaNaStenkuTestCase.h"

#include "TestCases/Components/ZanlungoCase.h"
#include "TestCases/Components/NavGraphTestCase.h"
#include "TestCases/Components/FsmTestCase.h"
#include "TestCases/Components/GoalShapeTestCase.h"

#include "TestCases/Groups/GroupMovementTestCase.h"
#include "TestCases/Groups/GroupPerformanceTestCase.h"
#include "TestCases/Groups/CorridorTestCase.h"


#include "Export/ComponentId.h"
#include "Export/Export.h"
#include "Util/RecordingSerializer.h"

#include "ThirdParty/date.h"

using namespace TestFusionCrowd;

using time_point = std::chrono::time_point<std::chrono::system_clock>;

void Run(ITestCase* testCase, std::vector<long long> & outMeasurements, time_point & outStartTime, std::string namePrefix)
{
	std::vector<long long> measurements;

	using namespace std::chrono;

	auto steps = testCase->GetStepCount();
	auto sim = testCase->GetSim();
	bool verobseSteps = testCase->VerboseSteps;

	sim->SetIsRecording(testCase->WriteTrajectories);

	std::ofstream steps_data(namePrefix + "_steps_data.csv");
	outStartTime = system_clock::now();
	for (size_t i = 0; i < steps; i++)
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		FusionCrowd::SimulationStepInfo stepInfo = sim->DoStep();
		steps_data << stepInfo.collisions << ", " << stepInfo.rewinds << ", " << stepInfo.totalSimulatedTime << std::endl;

		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		long long duration = duration_cast<microseconds>(t2 - t1).count();
		measurements.push_back(duration);

		if(verobseSteps && i % 10 == 0)
		{
			std::cout << "  Step " << i << "/" << steps
		              << " in " << duration << " microseconds"
		              << std::endl;
		}
	}

	outMeasurements.clear();
	std::copy(measurements.begin(), measurements.end(), std::back_inserter(outMeasurements));

	std::sort(measurements.begin(), measurements.end());

	std::cout << "  Step time stats:"
		<< " min=" << measurements[0]
		<< " Q1="  << measurements[steps / 4]
		<< " Q2="  << measurements[steps / 2]
		<< " Q3="  << measurements[steps * 3 / 4]
		<< " Q95=" << measurements[steps * 95 / 100]
		<< " max=" << measurements[steps - 1]
		<< std::endl;
}

std::string GetFileNamePrefix(ITestCase* testCase, time_point startTime, std::string folder, bool timeStamp = true)
{
	std::string d = timeStamp ? date::format("%H%M%S", startTime) + "_" : "";
	return folder + "\\" + d + testCase->GetName();
}

void WriteToFile(ITestCase* testCase, std::vector<long long> measurements, std::string namePrefix)
{
	if(testCase->WriteTime)
	{
		std::cout << "  1. Writing step times" << std::endl;
		std::ofstream time_measures(namePrefix + "_step_times_microsec.csv");

		for(auto val : measurements)
		{
			time_measures << val << std::endl;
		}
	}

	if(testCase->WriteTrajectories)
	{
		std::cout << "  2. Writing trajectories" << std::endl;

		std::string filename(namePrefix + "_trajs.csv");
		testCase->GetSim()->SerializeRecordingToFile(filename.c_str(), filename.size());
	}
}

int main()
{
	std::vector<std::shared_ptr<ITestCase>> cases =
	{
		// std::shared_ptr<ITestCase>((ITestCase*) new FsmTestCase(FusionCrowd::ComponentIds::BICYCLE, 50, 2000, true)),
		// std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, true)),
		// std::shared_ptr<ITestCase>((ITestCase*) new DebugTestCase(FusionCrowd::ComponentIds::KARAMOUZAS_ID, 100)),
		// std::shared_ptr<ITestCase>((ITestCase*) new ZanlungoCase()),
		// std::shared_ptr<ITestCase>((ITestCase*) new CrossingTestCase(FusionCrowd::ComponentIds::KARAMOUZAS_ID, 30, 1000, false)),
		// std::shared_ptr<ITestCase>((ITestCase*) new PinholeTestCase(FusionCrowd::ComponentIds::KARAMOUZAS_ID, 2, 100)),
		// std::shared_ptr<ITestCase>((ITestCase*) new TshapedFancyTestCase(FusionCrowd::ComponentIds::ORCA_ID, 4, 1000, true)),
		// std::shared_ptr<ITestCase>((ITestCase*) new NavGraphTestCase(100, 1000, true)),
		// std::shared_ptr<ITestCase>((ITestCase*) new StenkaNaStenkuTestCase(500, 1000, true)),
		// std::shared_ptr<ITestCase>((ITestCase*) new GroupMovementTestCase(1000, true)),
		// std::shared_ptr<ITestCase>((ITestCase*) new ExchangeCircleCase(7500, 1000, FusionCrowd::ComponentIds::ORCA_ID, false)),
		// std::shared_ptr<ITestCase>((ITestCase*) new GroupPerformanceTestCase()),
		// std::shared_ptr<ITestCase>((ITestCase*) new GoalShapeTestCase())
		// std::shared_ptr<ITestCase>((ITestCase*) new CorridorTestCase(30, .2f, 500)),
		//
		//
		/*std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::KARAMOUZAS_ID, 0, "Karamouzas_0")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::KARAMOUZAS_ID, 5, "Karamouzas_5")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::KARAMOUZAS_ID, 10, "Karamouzas_10")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::KARAMOUZAS_ID, 15, "Karamouzas_15")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::KARAMOUZAS_ID, 20, "Karamouzas_20")),

		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::HELBING_ID, 0, "Helbing_0")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::HELBING_ID, 5, "Helbing_5")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::HELBING_ID, 10, "Helbing_10")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::HELBING_ID, 15, "Helbing_15")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::HELBING_ID, 20, "Helbing_20")),

		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::ORCA_ID, 0, "ORCA_0")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::ORCA_ID, 5, "ORCA_5")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::ORCA_ID, 10, "ORCA_10")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::ORCA_ID, 15, "ORCA_15")),
		std::shared_ptr<ITestCase>((ITestCase*) new TradeshowTestCase(1025, 1000, FusionCrowd::ComponentIds::ORCA_ID, 20, "ORCA_20")),*/
	};

	struct CaseDescription
	{
		FusionCrowd::ComponentId component;
		std::string name;
	};

	std::vector<CaseDescription> allCases = {
		{FusionCrowd::ComponentIds::HELBING_ID, "Helbing_"},
		//{FusionCrowd::ComponentIds::ORCA_ID, "ORCA_"},
		//{FusionCrowd::ComponentIds::KARAMOUZAS_ID, "Karamouzas_"},
		//{FusionCrowd::ComponentIds::PEDVO_ID, "PEDVO_"},
	};

	std::vector<size_t> rewinds = { 0, 1, 4, 7, 10 };

	std::vector<long long> measurements;
	time_point startTime;

	std::string folderName = "Runs\\" + date::format("%Y%m%d", std::chrono::system_clock::now());
	_mkdir(folderName.c_str());

	for(auto desc : allCases)
	{
		for(auto r : rewinds) {
			TradeshowTestCase testCase(1000, 1000, desc.component, r, desc.name + std::to_string(r));
			std::string namePrefix = GetFileNamePrefix(&testCase, startTime, folderName, false);

			std::cout << "Case " << testCase.GetName() << std::endl
			          << " #agents=" << testCase.GetAgentCount() << ", #step=" << testCase.GetStepCount() << std::endl;

			std::cout << " 1. Setting up..." << std::endl;
			testCase.Pre();
			std::cout << "  Done." << std::endl;

			std::cout << " 2. Running..." << std::endl;
			Run(&testCase, measurements, startTime, namePrefix);
			std::cout << "  Done." << std::endl;

			std::cout << " 3. Saving results..." << std::endl;
			WriteToFile(&testCase, measurements, namePrefix);
			std::cout << "  Done." << std::endl;

			std::cout << " 4. Cleaning up ..." << std::endl;
			testCase.Post();
			std::cout << "  Done." << std::endl;

			std::cout << std::endl << std::endl;
		}
	}
}
