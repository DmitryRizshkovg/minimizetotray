
RegisterLang();

lang_tr_TR = 
{
	lngLanguage: "Dil",
	
	lngMinimizeOn: "Sistem tepsisine g�nder",
	lngMinimizeOnLeftButton: "Simge durumuna k���lte sol t�kland���nda sistem tepsisine g�nder",
	lngMinimizeOnRightButton: "Simge durumuna k���lte sa� t�kland���nda sistem tepsisine g�nder",
		
	lngTrayIcon: "Sistem tepsisi",
	lngHideTrayIcon: "E�er simge durumuna k���lt�lm�� Chrome penceresi yoksa sistem tepsisinde g�sterme",
		
	lngMouseActions: "Fare T�klamalar�",
	lngLeftButton: "Sol Tu�",
	lngDoubleClick: "�ift T�k",
	lngRightButton: "Sa� Tu�",
	
	lngContextMenu: "��erik Men�s�",	
	lngNewWindow: "'Yeni Pencere' A�",
	lngNewTab: "'Yeni Sekme' A�",
	lngOptions: "'Ayarlar'� A�",
	lngFavorites: "'Favoriler'i A�",
		
	lngSave: "Kaydet",
	lngExit: "��k��",
		
	lngDoNothing: "Hi�bir �ey yapma",
	lngShowContextMenu : "��erik men�s�n� g�ster",
	lngRestoreAllWindows: "T�m k���lt�lm�� pencereleri a�",
	lngCreateNewTab: "Yeni Sekme a�",
	lngCreateNewWindow: "Yeni Pencere a�"
		
	lngBossKey: "Acil durum k�sayolu",
	lngEnableBossKey: "T�m Chrome pencerelerini gizle",
	lngBossHideTrayIcon: "Sistem tepsisi simgesini gizle",
		
	lngRestoreAllHotkey: "T�m pencereleri geri b�y�t",
	lngEnableRestoreAllHotkey: "T�m Chrome pencerelerini geri b�y�t"
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
	
	opt.text = "T�rk�e";
	opt.value = "tr_TR";
	
	ctrl.add(opt);
}