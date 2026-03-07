/**
 * EnableInventoryInVehicle - BAE-Z
 *
 * Allows inventory access while in vehicles. Replaces removed Workshop mod
 * (3594596641) with a dog-mod-safe implementation — checks for open
 * scripted menus before re-locking inventory so DayZ-Dog's
 * DogManageMenu doesn't get stuck.
 */

modded class PlayerBase
{
    override void OnCommandVehicleStart()
    {
        super.OnCommandVehicleStart();
        if (GetInventory())
            GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
    }

    override void OnCommandVehicleFinish()
    {
        // Guard: don't re-lock if a scripted menu (e.g. DogManageMenu) is open,
        // otherwise the menu captures input but inventory lock prevents interaction
        if (GetInventory() && !GetGame().GetUIManager().GetMenu())
            GetInventory().LockInventory(LOCK_FROM_SCRIPT);
        super.OnCommandVehicleFinish();
    }

    override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
    {
        if (IsInVehicle())
            return true;
        return super.CanReceiveItemIntoHands(item_to_hands);
    }

    // Fix for SurvivorAnims crash: its ModCommandHandlerAfter calls
    // AnimSetInt with a bad handle on the intro scene mannequin.
    // Block the dangerous path (MOVE command on non-player entities),
    // let super (SurvivorAnims) run normally for the actual player.
    override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
    {
        if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE && GetGame().GetPlayer() != this)
            return true;

        return super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished);
    }
}
