﻿NDSummary.OnToolTipsLoaded("File:ssGUI/GUIObjectClasses/Text.hpp",{869:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype869\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">ssGUI</div></div></div></div>",583:"<div class=\"NDToolTip TClass LC\"><div class=\"TTSummary\">A class for showing text with different options such as font size, text alignment, fonts, etc.&nbsp; Text can be added by just setting the text, which is the simplest.</div></div>",585:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype585\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ComputeCharactersPositionAndSize()</div></div><div class=\"TTSummary\">Computes all the characters\' positions and sizes. This is called automatically when there\'s any changes to the text.</div></div>",586:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype586\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetText(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PName last\">text</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the text to show</div></div>",587:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype587\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetText(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">text</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the text to show</div></div>",588:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype588\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::wstring GetText() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the text being shown</div></div>",589:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype589\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetCharacterCount() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the number of characters for the text being shown</div></div>",598:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype598\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> ssGUI::CharacterRenderInfo GetCharacterRenderInfo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the character render info of the character at the index position</div></div>",599:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype599\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetOverrideCharacterDetails(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">CharacterDetails&nbsp;</td><td class=\"PName last\">details</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the override character details of the character at the index position</div></div>",600:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype600\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> ssGUI::CharacterDetails GetOverrideCharacterDetails(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the override character details of the character at the index position</div></div>",601:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype601\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetOverrideCharactersDetailsCount() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the number of override characater details</div></div>",602:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype602\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddOverrideCharacterDetails(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">CharacterDetails&nbsp;</td><td class=\"PName last\">details</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Add the override character details of the character at the index position</div></div>",603:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype603\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddOverrideCharacterDetails(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">CharacterDetails&nbsp;</td><td class=\"PName last\">details</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Add the override character details of the character to the end</div></div>",609:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype609\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RemoveOverrideCharacterDetails(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes the override character details of the character at the index position</div></div>",610:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype610\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ClearAllOverrideCharacterDetails()</div></div><div class=\"TTSummary\">Removes all override character details</div></div>",611:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype611\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> glm::vec2 GetCharacterGlobalPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">topLeftCorner</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Gets the global position of the character If topLeftCorner is true, this will return the top-left corner of the character, Otherwise this will return the left-most position of the character on the horizontal line.</div></div>",612:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype612\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsOverflow() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns true if the text is overflowing the text widget</div></div>",613:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype613\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFontSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">size</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the size of the font being used</div></div>",615:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype615\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetFontSize() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the size of the font being used</div></div>",616:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype616\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTextColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">u8vec4&nbsp;</td><td class=\"PName last\">color</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the text color being used</div></div>",617:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype617\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::u8vec4 GetTextColor() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the text color being used</div></div>",618:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype618\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMultilineAllowed(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">multiline</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">If true, newlines will be allowed</div></div>",619:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype619\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsMultilineAllowed() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">If true, newlines will be allowed</div></div>",620:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype620\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetWrappingMode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Enums::</td><td class=\"PType\">TextWrapping&nbsp;</td><td class=\"PName last\">wrappingMode</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the wrapping mode of this text widget</div></div>",621:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype621\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::TextWrapping GetWrappingMode() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the wrapping mode of this text widget</div></div>",622:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype622\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetHorizontalAlignment(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Enums::</td><td class=\"PType\">TextAlignmentHorizontal&nbsp;</td><td class=\"PName last\">align</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",623:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype623\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::TextAlignmentHorizontal GetHorizontalAlignment() <span class=\"SHKeyword\">const</span></div></div></div>",775:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype775\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVerticalAlignment(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Enums::</td><td class=\"PType\">TextAlignmentVertical&nbsp;</td><td class=\"PName last\">align</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",776:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype776\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::TextAlignmentVertical GetVerticalAlignment() <span class=\"SHKeyword\">const</span></div></div></div>",777:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype777\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Font*&nbsp;</td><td class=\"PName last\">font</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds the font to the end for this text object. Multiple fonts can be added as &quot;fall back&quot; if the character is not supported by it.</div></div>",778:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype778\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Font*&nbsp;</td><td class=\"PName last\">font,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds the font to the index position for this text object. Multiple fonts can be added as &quot;fall back&quot; if the character is not supported by it.</div></div>",779:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype779\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> ssGUI::Font* GetFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div></div>",780:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype780\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Font*&nbsp;</td><td class=\"PName last\">font,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",781:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype781\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RemoveFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",1368:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1368\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetFontsCount() <span class=\"SHKeyword\">const</span></div></div></div>",1401:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1401\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetHorizontalPadding(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">padding</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the horizontal padding for the beginning and the end of the text</div></div>",1402:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1402\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetHorizontalPadding() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the horizontal padding for the beginning and the end of the text</div></div>",1403:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1403\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVerticalPadding(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">padding</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the vertical padding for the beginning and the end of the text</div></div>",1404:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1404\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetVerticalPadding() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Sets the vertical padding for the beginning and the end of the text</div></div>",1405:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1405\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCharacterSpace(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">charSpace</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the space between each character</div></div>",1406:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1406\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetCharacterSpace() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the space between each character</div></div>",1407:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1407\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetLineSpace(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">lineSpace</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the space between each line</div></div>",1408:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1408\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetLineSpace() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the space between each line</div></div>",1409:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1409\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTabSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">tabSize</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets how many space each tab is</div></div>",1410:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1410\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetTabSize() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets how many space each tab is</div></div>",1411:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1411\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static void</span> AddDefaultFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Font*&nbsp;</td><td class=\"PName last\">font</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds the font to the end of default fonts. Multiple fonts can be added as &quot;fall back&quot; if the character is not supported by it.</div></div>",1412:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1412\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static void</span> AddDefaultFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Font*&nbsp;</td><td class=\"PName last\">font,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds the font to the index position of default fonts. Multiple fonts can be added as &quot;fall back&quot; if the character is not supported by it.</div></div>",1413:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1413\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static</span> ssGUI::Font* GetDefaultFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",1414:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1414\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static void</span> SetDefaultFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Font*&nbsp;</td><td class=\"PName last\">font,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",1415:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1415\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static void</span> RemoveDefaultFont(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",1416:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1416\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::GUIObjectType GetType() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">See GUIObject::GetType</div></div>",1417:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1417\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Internal_Update(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Backend::</td><td class=\"PType\">BackendSystemInputInterface*&nbsp;</td><td class=\"PName last\">inputInterface,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">InputStatus&amp;&nbsp;</td><td class=\"PName last\">globalInputStatus,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">InputStatus&amp;&nbsp;</td><td class=\"PName last\">windowInputStatus,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">mainWindow</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See GUIObject::Internal_Update</div></div>",1418:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1418\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> Text* Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">cloneChildren</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See GUIObject::Clone</div></div>"});