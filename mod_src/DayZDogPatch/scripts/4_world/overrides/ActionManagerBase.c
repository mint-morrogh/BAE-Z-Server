modded class ActionManagerBase
{
	override bool ActionPossibilityCheck(int pCurrentCommandID)
	{	
		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			return true;
		}
		return super.ActionPossibilityCheck( pCurrentCommandID );
		
	}
}
