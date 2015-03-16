
RegisterLang();

lang_tr_TR = 
{
	lngLanguage: "Dil",
		
	lngTrayIcon: "Sistem tepsisi",
	lngHideTrayIcon: "E�er simge durumuna k���lt�lm�� Chrome penceresi yoksa sistem tepsisinde g�sterme",
		
	lngMouseActions: "Fare T�klamalar�",
	lngLeftButton: "Sol Tu�",
	lngDoubleClick: "�ift T�k",
	lngRightButton: "Sa� Tu�",
	
	lngContextMenu: "��erik Men�s�",	
	lngNewWindow: "'Yeni Pencere' A�",
	lngNewTab: "'Yeni Sekme' A�",
		
	lngSave: "Kaydet",
	lngExit: "��k��",
		
	lngDoNothing: "Hi�bir �ey yapma",
	lngShowContextMenu : "��erik men�s�n� g�ster",
	lngRestoreAllWindows: "T�m k���lt�lm�� pencereleri a�",
	lngCreateNewTab: "Yeni Sekme a�",
	lngCreateNewWindow: "Yeni Pencere a�"
}

function RegisterLang()
{
	var ctrl = document.getElementById("language");
	
	if(ctrl != null)
	{
		ctrl.add(createOption("T�rk�e", "tr_TR"));
	}
}