
RegisterLang();

lang_tr_TR = 
{
	lngLanguage: "Dil",
		
	lngTrayIcon: "Sistem tepsisi",
	lngHideTrayIcon: "Eðer simge durumuna küçültülmüþ Chrome penceresi yoksa sistem tepsisinde gösterme",
		
	lngMouseActions: "Fare Týklamalarý",
	lngLeftButton: "Sol Tuþ",
	lngDoubleClick: "Çift Týk",
	lngRightButton: "Sað Tuþ",
	
	lngContextMenu: "Ýçerik Menüsü",	
	lngNewWindow: "'Yeni Pencere' Aç",
	lngNewTab: "'Yeni Sekme' Aç",
		
	lngSave: "Kaydet",
	lngExit: "Çýkýþ",
		
	lngDoNothing: "Hiçbir þey yapma",
	lngShowContextMenu : "Ýçerik menüsünü göster",
	lngRestoreAllWindows: "Tüm küçültülmüþ pencereleri aç",
	lngCreateNewTab: "Yeni Sekme aç",
	lngCreateNewWindow: "Yeni Pencere aç"
}

function RegisterLang()
{
	var ctrl = document.getElementById("language");
	
	if(ctrl != null)
	{
		ctrl.add(createOption("Türkçe", "tr_TR"));
	}
}