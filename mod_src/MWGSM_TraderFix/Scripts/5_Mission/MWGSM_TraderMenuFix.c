modded class MWGSM_TraderMenu
{
	override Widget Init()
	{
		Widget root = super.Init();

		TextWidget saldoLabel = TextWidget.Cast(root.FindAnyWidget("text_saldo"));
		if (saldoLabel)
		{
			saldoLabel.SetText("Roubles: ");
		}

		return root;
	}
};
