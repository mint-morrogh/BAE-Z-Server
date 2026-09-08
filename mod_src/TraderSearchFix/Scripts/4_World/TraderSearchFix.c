/**
 * TraderSearchFix - BAE-Z
 *
 * Dr Jones Trader 1.9: typing in the trader search box crashes the client with an
 * out-of-memory error (reproduced on the Weapon Trader by typing "smoke").
 *
 * Cause: TraderMenu.Update() re-runs SearchForItems() on every keystroke, which
 * ends in SelectRow(0). The changed row makes Update() call updateItemPreview(),
 * and that spawns a real 3D entity with GetGame().CreateObject(). The previous
 * preview is released with ObjectDelete(), which is deferred, so a five-character
 * search queues up five full item models - weapon models on the gun trader - and
 * the client runs out of memory before the engine frees any of them.
 *
 * Fix: debounce the preview. updateItemPreview() now only records what should be
 * shown and re-arms a 300 ms timer; each new selection cancels the pending one, so
 * a burst of keystrokes creates exactly one entity instead of one per character.
 * The search list itself is untouched - it only reads configs and is not the leak.
 */
modded class TraderMenu
{
	protected string m_TSF_PendingType = "";
	protected bool   m_TSF_Building    = false;

	override void updateItemPreview(string itemType)
	{
		// re-entrant call from the timer below: this is the one that really builds
		if (m_TSF_Building)
		{
			super.updateItemPreview(itemType);
			return;
		}

		m_TSF_PendingType = itemType;

		// drop the model the old selection left behind and blank the panel, so the
		// preview never shows a stale item while the new one is pending
		if (previewItem)
		{
			GetGame().ObjectDelete(previewItem);
			previewItem = NULL;
		}
		if (m_ItemPreviewWidget)
			m_ItemPreviewWidget.SetItem(NULL);

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(TSF_BuildPreview);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(TSF_BuildPreview, 300, false);
	}

	void TSF_BuildPreview()
	{
		if (m_TSF_PendingType == "")
			return;

		// the row can be gone by now (category switched, search narrowed further)
		if (!m_ListboxItems || !m_FilteredListOfTraderItems)
			return;
		int row = m_ListboxItems.GetSelectedRow();
		if (row < 0 || row >= m_FilteredListOfTraderItems.Count())
			return;

		m_TSF_Building = true;
		updateItemPreview(m_TSF_PendingType);
		m_TSF_Building = false;
	}

	override void OnHide()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(TSF_BuildPreview);
		m_TSF_PendingType = "";
		super.OnHide();
	}
}
