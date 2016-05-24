#include "stdafx.h"
#include "Menu.h"
#include "BodyConstructor.h"

void Menu()
{
	CMenu menu;
	CBodyConstructor constructor(std::cin);

	menu.AddItem("compound", "Create compound <id>",
		[&] {constructor.CreateBody(BodyType::Compound); });
	menu.AddItem("cone", "Create cone <id density radius height>(g/cm3, cm)",
		[&] {constructor.CreateBody(BodyType::Cone); });
	menu.AddItem("cylinder", "Create cylinder <id density radius height>(g/cm3, cm)",
		[&] {constructor.CreateBody(BodyType::Cylinder); });
	menu.AddItem("parallelepiped", "Create parallelepiped <id density width height depth>(g/cm3, cm)",
		[&] {constructor.CreateBody(BodyType::Parallelepiped); });
	menu.AddItem("sphere", "Create sphere <id density radius>(g/cm3, cm)",
		[&] {constructor.CreateBody(BodyType::Sphere); });
	menu.AddItem("move", "Move body <idReceiver idMoved>",
		[&] {constructor.MoveBody(); });
	menu.AddItem("list", "Bodies List",
		[&] {constructor.BodiesList(); });
	menu.AddItem("help", "Show instructions",
		[&] {menu.ShowInstructions(); });
	menu.AddItem("exit", "Exit from this menu",
		[&] {menu.Exit(); });

	menu.Run();
}

int main()
{
	Menu();

	return 0;
}
