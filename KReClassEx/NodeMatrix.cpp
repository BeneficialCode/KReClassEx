#include "stdafx.h"
#include "NodeMatrix.h"

CNodeMatrix::CNodeMatrix() {
	m_NodeType = NodeType::Matrix;
}

void CNodeMatrix::Update(const PHOTSPOT spot) {
	float value;

	StandardUpdate(spot);

	if (spot->Id < 16) {
		value = (float)_ttof(spot->Text);
		ReClassWriteMemory(spot->Address + (spot->Id * sizeof(float)), &value, sizeof(float));
	}


}

NODESIZE CNodeMatrix::Draw(const PVIEWINFO view, int x, int y)
{
    int tx, mx;
    NODESIZE drawSize;
    float* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (float*)(view->Data + m_Offset);
    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_MATRIX, HS_NONE, HS_NONE);
    mx = tx;
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("Matrix "));
    tx = AddText(view, tx, y, g_clrName, 69, _T("%s"), m_Name);
    tx = AddOpenClose(view, tx, y);
    tx += g_FontWidth;
    tx = AddComment(view, tx, y);

    if (m_LevelsOpen[view->Level])
    {
        y += g_FontHeight;
        tx = mx;
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
        tx = AddText(view, tx, y, g_clrValue, 0, _T("% 14.3f"), pData[0]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 1, _T("% 14.3f"), pData[1]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 2, _T("% 14.3f"), pData[2]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 3, _T("% 14.3f"), pData[3]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
        y += g_FontHeight;
        tx = mx;
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
        tx = AddText(view, tx, y, g_clrValue, 4, _T("% 14.3f"), pData[4]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 5, _T("% 14.3f"), pData[5]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 6, _T("% 14.3f"), pData[6]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 7, _T("% 14.3f"), pData[7]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
        y += g_FontHeight;
        tx = mx;
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
        tx = AddText(view, tx, y, g_clrValue, 8, _T("% 14.3f"), pData[8]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 9, _T("% 14.3f"), pData[9]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 10, _T("% 14.3f"), pData[10]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 11, _T("% 14.3f"), pData[11]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
        y += g_FontHeight;
        tx = mx;
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
        tx = AddText(view, tx, y, g_clrValue, 12, _T("% 14.3f"), pData[12]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 13, _T("% 14.3f"), pData[13]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 14, _T("% 14.3f"), pData[14]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 15, _T("% 14.3f"), pData[15]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("|"));
    }

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}