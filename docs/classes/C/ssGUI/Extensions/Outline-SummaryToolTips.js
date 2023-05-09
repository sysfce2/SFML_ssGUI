﻿NDSummary.OnToolTipsLoaded("CClass:ssGUI.Extensions.Outline",{1737:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1737\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;Extensions::</span>&#8203;Outline</div></div></div><div class=\"TTSummary\">Outline allows to create a colored outline surrounding the target GUI Object shape/vertices.</div></div>",1739:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1739\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetOutlineThickness(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">thickness</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the thickness of the outline, in pixel</div></div>",1740:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1740\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetOutlineThickness() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the thickness of the outline, in pixel</div></div>",1741:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1741\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetInnerOutline(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">inner</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if the outlines are drawn inside the shape or not</div></div>",1742:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1742\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsInnerOutline() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if the outlines are drawn inside the shape or not</div></div>",1743:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1743\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetSimpleOutline(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">simpleOutline</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if it uses simple outline drawing or not. Note this has no effect if IsInnerOutline is true.</div></div>",1744:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1744\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsSimpleOutline() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if it uses simple outline drawing or not. Note this has no effect if IsInnerOutline is true.</div></div>",1745:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1745\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetOutlineColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">u8vec4&nbsp;</td><td class=\"PName last\">color</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the color of the outline</div></div>",1746:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1746\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::u8vec4 GetOutlineColor() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the color of the outline</div></div>",1747:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1747\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddTargetShape(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">shapeIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds the shapeIndex that indicates the index of the shape to be outlined in GUI object.&nbsp; Note that if you have added any target vertices (AddTargetVertex), this will be <b>ignored</b>.</div></div>",1748:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1748\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual int</span> GetTargetShape(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">location</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the shapeIndex that indicates the index of the shape to be outlined in GUI object, by specifying the location shapedIndex that is stored in this extension.</div></div>",1749:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1749\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTargetShape(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">location,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">shapeIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the shapeIndex that indicates the index of the shape to be outlined in GUI object, by specifying the location shapedIndex that is stored in this extension.</div></div>",1750:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1750\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetTargetShapesCount() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the number of shapes to be outlined (number of shapeIndex) that are stored in this extension.</div></div>",1751:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1751\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RemoveTargetShape(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">location</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes the shapeIndex that indicates the index of the shape to be outlined in GUI object, by specifying the location shapedIndex that is stored in this extension.</div></div>",1752:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1752\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ClearTargetShapes()</div></div><div class=\"TTSummary\">Clears all the shapeIndex entries in this extension</div></div>",1753:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1753\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddTargetVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">vertexIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds the target vertex that indicates the index of the vertex to be outlined in GUI object, by specifying the location vertexIndex that is stored in this extension.&nbsp; Note that if you are adding any target vertices, any target shapes added (AddTargetShape) this will be ignored.</div></div>",1754:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1754\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual int</span> GetTargetVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">location</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the vertexIndex that indicates the index of the vertex to be outlined in GUI object, by specifying the location vertexIndex that is stored in this extension.</div></div>",1755:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1755\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTargetVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">location,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">vertexIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the vertexIndex that indicates the index of the vertex to be outlined in GUI object, by specifying the location vertexIndex that is stored in this extension.</div></div>",1756:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1756\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetTargetVerticesCount() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the number of indices to be outlined (number of vertexIndex) that are stored in this extension.</div></div>",1757:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1757\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RemoveTargetVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">location</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes the vertexIndex that indicates the index of the vertex to be outlined in GUI object, by specifying the location vertexIndex that is stored in this extension.</div></div>",1758:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1758\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ClearTargetVertices()</div></div><div class=\"TTSummary\">Clears all the vertexIndex entries in this extension</div></div>",1759:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1759\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsEnabled() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">See Extension::IsEnabled</div></div>",1760:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1760\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Internal_Update(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">isPreUpdate,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::Backend::</td><td class=\"PType\">BackendSystemInputInterface*&nbsp;</td><td class=\"PName last\">inputInterface,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">InputStatus&amp;&nbsp;</td><td class=\"PName last\">inputStatus,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">mainWindow</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See Extension::Internal_Update</div></div>",1761:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1761\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Internal_Draw(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">isPreRender,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::Backend::</td><td class=\"PType\">BackendDrawingInterface*&nbsp;</td><td class=\"PName last\">drawingInterface,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">mainWindow,</td></tr><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">mainWindowPositionOffset</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See Extension::Internal_Draw</div></div>",1762:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1762\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetExtensionName() <span class=\"SHKeyword\">override</span></div></div><div class=\"TTSummary\">See Extension::GetExtensionName</div></div>",1763:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1763\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BindToObject(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">bindObj</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See Extension::BindToObject</div></div>",1764:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1764\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Copy(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Extensions::</td><td class=\"PType\">Extension*&nbsp;</td><td class=\"PName last\">extension</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See Extension::Copy</div></div>",1765:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1765\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ObjectsReferences* Internal_GetObjectsReferences() <span class=\"SHKeyword\">override</span></div></div><div class=\"TTSummary\">See Extension::Internal_GetObjectsReferences</div></div>",1766:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1766\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Outline* Clone() <span class=\"SHKeyword\">override</span></div></div><div class=\"TTSummary\">See Extension::Clone</div></div>"});