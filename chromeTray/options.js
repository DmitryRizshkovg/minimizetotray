init();

function init()
{
	setSelectedValue("language", readProperty("language", "en_US"));
	localize();
		
	fillActionSelect('actLClick');
	fillActionSelect('actLDblClick');
	fillActionSelect('actRClick');
	
	doDataExchange(false);
	
	document.getElementById("lngSave").addEventListener("click", onSave);
	document.getElementById("lngExit").addEventListener("click", onExit);
}

function getSelectedLanguage()
{
	var lang = getSelectedValue("language");
	
	if(lang = "en_US")
	{
		return lang_en_US;
	}
	else if(lang = "es_ES")
	{
		return lang_es_ES;
	}
	else if(lang = "ru_RU")
	{
		return lang_ru_RU;
	}
	else if(lang = "tr_TR")
	{
		return lang_tr_TR;
	}
	
	return lang_en_US;
}

function onLanguageSelected()
{
	doDataExchange(true);
	init();
}

function localize()
{
	var lang  = getSelectedLanguage();
	
	for(var ctrlId in lang)
	{
   		var value = lang[ctrlId];
   		
   		var ctrl = document.getElementById(ctrlId);
   		
   		if(ctrl == null)
   		{
   			continue;
   		}
   		
   		if(ctrl.tagName == "SPAN")
   		{
   			ctrl.innerText = value;
   		}
   		else if(ctrl.tagName == "INPUT")
   		{
   			ctrl.value = value;
   		}
	}
}

function fillActionSelect(selectId)
{
	var lang  = getSelectedLanguage();

	var ctrl = document.getElementById(selectId);
	
	while(ctrl.options.length != 0)
	{      
		ctrl.removeChild(ctrl.options[0]);
	}
	
	ctrl.add(createOption(lang.lngDoNothing, "0"));
	ctrl.add(createOption(lang.lngShowContextMenu, "1"));
	ctrl.add(createOption(lang.lngRestoreAllWindows, "2"));
	ctrl.add(createOption(lang.lngCreateNewTab, "3"));
	ctrl.add(createOption(lang.lngCreateNewWindow, "4"));
}

function createOption(text, value)
{
	var opt = document.createElement('option');
	
	opt.text = text;
	opt.value = value;
	
	return opt;
}

function getSelectedValue(selectId)
{
	var ctrl = document.getElementById(selectId);
	return ctrl.options[ctrl.selectedIndex].value;
}

function setSelectedValue(selectId, value)
{
	var ctrl = document.getElementById(selectId);
	
	for (var i = 0; i < ctrl.options.length; ++i)
	{
		if (ctrl.options[i].value == value)
		{
			ctrl.options[i].selected = true;
			break;
		}
	}
}

function getCheckState(ctrlId)
{
	return document.getElementById(ctrlId).checked;
}

function setCheckState(ctrlId, checked)
{
	document.getElementById(ctrlId).checked = (checked == "true");
}

function getValue(ctrlId)
{
	var ctrl = document.getElementById(ctrlId);
	return ctrl.value;
}

function setValue(ctrlId, value)
{
	var ctrl = document.getElementById(ctrlId);
	ctrl.value = value;
}

function readProperty(property, defValue)
{
	if(localStorage == null)
	{
		return defValue;
	}
	
	if(localStorage[property] == null)
	{
		return defValue;
	}
	
	return localStorage[property];
}

function doDataExchange(save)
{
	if(save)
	{
		localStorage["language"] = getSelectedValue("language");
		
		localStorage["minimizeOnLeftButton"] = getCheckState("minimizeOnLeftButton");
		localStorage["minimizeOnRightButton"] = getCheckState("minimizeOnRightButton");

		localStorage["trayHide"] = getCheckState("trayHide");
		
		localStorage["actLClick"] = getSelectedValue("actLClick");
		localStorage["actLDblClick"] = getSelectedValue("actLDblClick");
		localStorage["actRClick"] = getSelectedValue("actRClick");
		
		localStorage["displayNewWindow"] = getCheckState("displayNewWindow");
		localStorage["displayNewTab"] = getCheckState("displayNewTab");
		localStorage["displayOptions"] = getCheckState("displayOptions");
		localStorage["displayFavorites"] = getCheckState("displayFavorites");
		localStorage["favorites"] = getValue("favorites");
		
		localStorage["enableBossKey"] = getCheckState("enableBossKey");
		localStorage["bossMod1"] = getSelectedValue("bossMod1");
		localStorage["bossMod2"] = getSelectedValue("bossMod2");
		localStorage["bossKey"] = getValue("bossKey");
		localStorage["bossHideTrayIcon"] = getCheckState("bossHideTrayIcon");
				
		localStorage["enableRestoreAllHotkey"] = getCheckState("enableRestoreAllHotkey");
		localStorage["restoreAllHotkeyMod1"] = getSelectedValue("restoreAllHotkeyMod1");
		localStorage["restoreAllHotkeyMod2"] = getSelectedValue("restoreAllHotkeyMod2");
		localStorage["restoreAllHotkeyKey"] = getValue("restoreAllHotkeyKey");
	}
	else
	{
		setSelectedValue("language", readProperty("language", "en_US"));
		
		setCheckState("minimizeOnLeftButton", readProperty("minimizeOnLeftButton", "true"));
		setCheckState("minimizeOnRightButton", readProperty("minimizeOnRightButton", "false"));

		setCheckState("trayHide", readProperty("trayHide", "false"));
		
		setSelectedValue("actLClick", readProperty("actLClick", "0"));
		setSelectedValue("actLDblClick", readProperty("actLDblClick", "2"));
		setSelectedValue("actRClick", readProperty("actRClick", "1"));
		
		setCheckState("displayNewWindow", readProperty("displayNewWindow", "false"));
		setCheckState("displayNewTab", readProperty("displayNewTab", "false"));
		setCheckState("displayOptions", readProperty("displayOptions", "true"));
		setCheckState("displayFavorites", readProperty("displayFavorites", "false"));
		setValue("favorites", readProperty("favorites", ""));
		
		setCheckState("enableBossKey", readProperty("enableBossKey", "false"));
		setSelectedValue("bossMod1", readProperty("bossMod1", "Ctrl"));
		setSelectedValue("bossMod2", readProperty("bossMod2", "Shift"));
		setValue("bossKey", readProperty("bossKey", ""));
		setCheckState("bossHideTrayIcon", readProperty("bossHideTrayIcon", "false"));
		
		setCheckState("enableRestoreAllHotkey", readProperty("enableRestoreAllHotkey", "false"));
		setSelectedValue("restoreAllHotkeyMod1", readProperty("restoreAllHotkeyMod1", "Ctrl"));
		setSelectedValue("restoreAllHotkeyMod2", readProperty("restoreAllHotkeyMod2", "Shift"));
		setValue("restoreAllHotkeyKey", readProperty("restoreAllHotkeyKey", ""));
	}
}

function onSave()
{
	doDataExchange(true);
	
	var port = chrome.extension.connect({name: 'minimize-to-tray'});
	port.postMessage({command: 'OptionsChanged'});
	
	window.close();
}

function onExit()
{
	window.close();
}