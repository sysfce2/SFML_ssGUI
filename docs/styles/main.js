﻿"use strict";var NDLoader=new function(){this.JSLinks_All=["DefaultJS/NDCore.js","CustomStyleImproved/RenameFileToMenu.js"];this.JSLinks_Frame=["DefaultJS/NDFramePage.js","DefaultJS/NDMenu.js","DefaultJS/NDSummary.js","DefaultJS/NDSearch.js"];this.JSLinks_Content=["DefaultJS/NDContentPage.js"];this.JSLinks_Home=[];this.LoadJS=function(pageType,relativePrefix){this.LoadJSArray(this.JSLinks_All,relativePrefix);this.LoadJSArray(this['JSLinks_'+pageType],relativePrefix);};this.LoadJSArray=function(links,relativePrefix){if(navigator.userAgent.indexOf('KHTML')!=-1){for(var i=0;i<links.length;i++){document.write('<script type="text/javascript" src="'+relativePrefix+links[i]+'"></script>');}}else{var head=document.getElementsByTagName('head')[0];for(var i=0;i<links.length;i++){var script=document.createElement('script');script.src=relativePrefix+links[i];script.type='text/javascript';head.appendChild(script);}}};this.OnLoad=function(pageType){this.OnLoad_All();this['OnLoad_'+pageType]();};this.OnLoad_All=function(){NDCore.AddIEClassesToBody();};this.OnLoad_Frame=function(){NDFramePage.Start();};this.OnLoad_Content=function(){NDContentPage.Start();};this.OnLoad_Home=function(){};};