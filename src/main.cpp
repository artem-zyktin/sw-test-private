#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>
#include <fstream>
#include <iostream>

#include <Features.hpp>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	auto configRegistry = std::make_unique<UnitConfigRegistry>(std::make_unique<core::UnitFactory>());
	configRegistry->Register(HunterConfiguration{}, "Hunter");
	configRegistry->Register(SwordsmanConfiguration{}, "Swordsman");

	auto eventLog = std::make_shared<EventLog>();

	auto game = Game(std::move(configRegistry), eventLog);

	std::cout << "Commands:\n";
	io::CommandParser parser;
	parser.add<io::CreateMap>([&game](auto command) mutable { game.CreateMap(command.width, command.height); })
		.add<io::SpawnSwordsman>([&game](auto command) mutable { game.SpawnUnit("Swordsman", command.unitId, DataPack{ core::Position{int(command.x), int(command.y)},
																													   HealthData{int(command.hp), int(command.hp)},
																													   ActionControllerData{},
																													   MovementData{1},
																													   MeleeAttackData{int(command.strength)}}); })
		.add<io::SpawnHunter>([&game](auto command) mutable { game.SpawnUnit("Hunter", command.unitId, DataPack{ core::Position{int(command.x), int(command.y)},
																												 HealthData{int(command.hp), int(command.hp)},
																												 ActionControllerData{},
																												 MovementData{1},
																												 MeleeAttackData{int(command.strength)},
																												 ShootingAttackData{int(command.agility), 2, int(command.range)} }); })
		.add<io::March>([&game](auto command) mutable { game.StartMarch(command.unitId, core::Position{ int(command.targetX), int(command.targetY) }); });

	parser.parse(file);

	game.Run();

	return 0;
}
