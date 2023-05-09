﻿NDContentPage.OnToolTipsLoaded({515:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype515\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;CharacterRenderInfo</div></div></div></div>",529:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype529\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;Font</div></div></div></div>",576:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype576\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;ImageData</div></div></div><div class=\"TTSummary\">If you are using LoadRawFromMemory, remember to reload it again if there\'s any changes to the image memory.</div></div>",1098:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1098\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;Backend::</span>&#8203;BackendFontInterface</div></div></div></div>",1101:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1101\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> ssGUI::CharacterRenderInfo GetCharacterRenderInfo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PName last\">charUnicode,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">charSize</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the rendering infomation of a character.&nbsp; For fixed size font behaviour, please see the backend you are using.</div></div>",1102:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1102\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> IsCharacterSupported(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PName last\">charUnicode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns true if the character is supported</div></div>",1103:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1103\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual float</span> GetKerning(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PName last\">charUnicode,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PName last\">secondCharUnicode,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">charSize</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Kerning represents the drawing position offset when drawn after certain characters.&nbsp; (i.e. AV has a negative kerning for the characte V)</div></div>",1104:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1104\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual float</span> GetLineSpacing(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">charSize</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns how much a line space is based on the font size.&nbsp; For fixed size font behaviour, please see the backend you are using.</div></div>",1105:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1105\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual float</span> GetUnderlineOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">charSize</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the vertical offset for underline based on the font size.&nbsp; For fixed size font behaviour, please see the backend you are using.</div></div>",1106:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1106\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual float</span> GetUnderlineThickness(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">charSize</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the thickness for underline based on the font size For fixed size font behaviour, please see the backend you are using.</div></div>",1107:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1107\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> LoadFromPath(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">path</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a font file. Supported font format is dependent on the backend.&nbsp; <u>path</u> can be either absolute or relative.</div></div>",1108:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1108\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> LoadFromMemory(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">void</span>*&nbsp;</td><td class=\"PName last\">dataPtr,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">lengthInBytes</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Load the raw font file in memory, which is copied and stored internally.</div></div>",1109:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1109\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> GetFixedAvailableFontSizes(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;<span class=\"SHKeyword\">float</span>&gt;&amp;&nbsp;</td><td class=\"PName last\">fontSizes</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">If the font is not scalable, this will return all the fixed sizes. Otherwise empty vector.</div></div>",1110:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1110\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> GetCharacterImage(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PName last\">charUnicode,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">charSize,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">ImageData&amp;&nbsp;</td><td class=\"PName last\">characterImage</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the character image For fixed size font behaviour, please see the backend you are using.</div></div>",1111:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1111\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span>* GetRawHandle() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the raw pointer to the underlying backend implmentation object</div></div>"});