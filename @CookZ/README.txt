A couple of parameters can be configured via a server config file. This file is generated with
default values at "$missionFolder\CookZ\CookZ.json" the first time you start the server with the
mod installed. After modifying the configuration, you need to restart the server.

(!) If the mod is updated and new configuration is added, then a file with an incremented version suffix is created,
    e.g. CookZ_V023.json. You need to transfer your changes from the previous version and restart your server –
    but you only need to keep the latest version.

The file has the following default content, enriched here with some documentation.

Boolean values are set by 1 for true and by 0 for false.

{
    // can the user craft the cookbook - for servers that want to add the book to the spawn pool or trader, set to 0
    "CanCraftCookBook": 1,

    // can the user craft empty cans - for servers that want to add empty cans to the spawn pool or trader, set to 0
    "CanCraftEmptyCans": 1,

    // can the user craft empty boxes - for servers that want to add empty boxes to the spawn pool or trader, set to 0
    "CanCraftEmptyBoxes": 1,

    // some ingredients have a quantity - this sets the minimum quantity to be used as an ingredient to 50%
    // must be 0 < MinIngredientQuantityPercent <= 100
    "MinIngredientQuantityPercent": 50,

    // the temperature a freshly created dish has - if kept longer in the cooking equipment, the temperature can still go up
    // must be between 0 and 100
    "TemperaturOfCreatedDish": 80.0,

    // if an ingredient has the brain agent, the chance that the created dish does NOT have the agent
    // must be between 0.0 and 1.0 - 1.0 -> 100% chance the agent is removed, 0.0 -> 0% chance the agent is removed
    // note: in vanilla this is basically set to 0.0
    "ChanceToRemoveBrainAgent": 1.0,

    // same as "ChanceToRemoveBrainAgent" for the heavy metal agent
    // note: in vanilla this is basically set to 0.0
    "ChanceToRemoveHeavyMetalAgent": 1.0,

    // same as "ChanceToRemoveBrainAgent" for the salmonella agent
    // note: in vanilla this is basically set to 0.4 (for some steak types)
    "ChanceToRemoveSalmonellaAgent": 1.0,

    // can cheese decay (cause food poisoning after some time)
    "EnableCheeseDecay": 0,

    // can sausages decay (cause food poisoning after some time)
    "EnableSausageDecay": 0,

    // can (open) boxes and cans decay (cause food poisoning after some time)
    "EnableBoxAndCanDecay": 1,

    // are dishes cooked instantly - this is mostly for debugging and should not be changed
    "InstantCook": 0
}
