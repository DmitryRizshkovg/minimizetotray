
RegisterLang();

lang_tr_TR = 
{
	lngLanguage: "Dil",
	
	lngMinimizeOn: "Sistem tepsisine gönder",
	lngMinimizeOnLeftButton: "Simge durumuna küçülte sol týklandýðýnda sistem tepsisine gönder",
	lngMinimizeOnRightButton: "Simge durumuna küçülte sað týklandýðýnda sistem tepsisine gönder",
		
	lngTrayIcon: "Sistem tepsisi",
	lngHideTrayIcon: "Eðer simge durumuna küçültülmüþ Chrome penceresi yoksa sistem tepsisinde gösterme",
		
	lngMouseActions: "Fare Týklamalarý",
	lngLeftButton: "Sol Tuþ",
	lngDoubleClick: "Çift Týk",
	lngRightButton: "Sað Tuþ",
	
	lngContextMenu: "Ýçerik Menüsü",	
	lngNewWindow: "'Yeni Pencere' Aç",
	lngNewTab: "'Yeni Sekme' Aç",
	lngOptions: "'Ayarlar'ý Aç",
	lngFavorites: "'Favoriler'i Aç",
		
	lngSave: "Kaydet",
	lngExit: "Çýkýþ",
		
	lngDoNothing: "Hiçbir þey yapma",
	lngShowContextMenu : "Ýçerik menüsünü göster",
	lngRestoreAllWindows: "Tüm küçültülmüþ pencereleri aç",
	lngCreateNewTab: "Yeni Sekme aç",
	lngCreateNewWindow: "Yeni Pencere aç"
		
	lngBossKey: "Acil durum kýsayolu",
	lngEnableBossKey: "Tüm Chrome pencerelerini gizle",
	lngBossHideTrayIcon: "Sistem tepsisi simgesini gizle",
		
	lngRestoreAllHotkey: "Tüm pencereleri geri büyüt",
	lngEnableRestoreAllHotkey: "Tüm Chrome pencerelerini geri büyüt"
}
function createOption(text, value)
{
	var opt = document.createElement('option');
	
	opt.text = text;
	opt.value = value;
	
	return opt;
}

function RegisterLang()
{
	var ctrl = document.getElementById("language");
	
	var opt = document.createElement('option');
	
	opt.text = "Türkçe";
	opt.value = "tr_TR";
	
	ctrl.add(opt);
}