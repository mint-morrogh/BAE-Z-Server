modded class MissionBase {
    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
            case DOG_SELECT_MENU:
                menu = new DogSelectMenu;
            break;
			case DOG_MAP_MENU:
				menu = new DogMapMenu;
			break;
			case DOG_MANAGE_MENU:
				menu = new DogManageMenu;
			break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
}