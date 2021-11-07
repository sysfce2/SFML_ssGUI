﻿NDSummary.OnToolTipsLoaded("File:ssGUI/Backend/Interfaces/BackendImageInterface.hpp",{542:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype542\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;Backend</div></div></div></div>",543:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype543\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">BackendImageInterface</div></div></div></div>",545:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype545\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsValid() <span class=\"SHKeyword\">const</span> = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns true if image is loaded</div></div>",546:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype546\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> LoadFromPath(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">path</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Please see BackendImageSFML::LoadFromPath for supported image format.</div></div>",547:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype547\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> LoadImgFileFromMemory(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">void</span>*&nbsp;</td><td class=\"PName last\">dataPtr,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">size_t&nbsp;</td><td class=\"PName last\">size</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Please see BackendImageSFML::LoadImgFileFromMemory for supported image format.</div></div>",548:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype548\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> LoadRawFromMemory(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">void</span>*&nbsp;</td><td class=\"PName last\">dataPtr,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">This loads an image in memory in the format of 32-bits rgba.</div></div>",549:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype549\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::ivec2 GetSize() <span class=\"SHKeyword\">const</span> = <span class=\"SHNumber\">0</span></div></div></div>",550:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype550\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> BackendImageInterface* Clone() = <span class=\"SHNumber\">0</span></div></div></div>"});