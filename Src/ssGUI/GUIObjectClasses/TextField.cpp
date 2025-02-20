#include "ssGUI/GUIObjectClasses/TextField.hpp"

#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"

#include "ssGUI/Factory.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/HeaderGroups/InputGroup.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    TextField::TextField(TextField const& other) : Text(other)
    {
        LastBlinkTime = other.LastBlinkTime;
        BlinkDuration = other.BlinkDuration;
        BlinkCaret = other.BlinkCaret;
        LastArrowNavStartTime = other.LastArrowNavStartTime;
        ArrowNavPauseDuration = other.ArrowNavPauseDuration;
        LastArrowNavTime = other.LastArrowNavTime;
        ArrowNavInterval = other.ArrowNavInterval;
    }

    int TextField::GetEndOfPreviousWord(int curIndex)
    {
        //Check if there's any valid character before current character
        int validCharBeforeIndex = -1;
        for(int i = curIndex - 1; i >= 0; i--)
        {
            if(GetCharacterRenderInfo(i).Valid)
            {
                validCharBeforeIndex = i;
                break;
            }
        }
        if(validCharBeforeIndex < 0)
            return curIndex;

        enum class characterType
        {
            LETTERS,
            NUM,
            SYMBOL,
            SPACE_TAB_NEWLINE,
        };

        auto getCharType = [&](wchar_t c)->characterType
        {
            uint32_t charInt = (uint32_t)c;
            
            //If letters
            if((charInt >= 65 && charInt <= 90) || (charInt >= 97 && charInt <= 122))
                return characterType::LETTERS;
            //If numbers
            if(charInt >= 48 && charInt <= 57)
                return characterType::NUM;
            //If Symbol
            if((charInt >= 33 && charInt <= 47) || (charInt >= 58 && charInt <= 64) || (charInt >= 91 && charInt <= 96) || (charInt >= 123 && charInt <= 126))
                return characterType::SYMBOL;
            //If space, tab or newline 
            if(charInt == 9 || charInt == 10 || charInt == 12 || charInt == 13 || charInt == 32)
                return characterType::SPACE_TAB_NEWLINE;

            return characterType::LETTERS;
        };

        characterType lastCharType = getCharType(GetInternalCharacterDetail(validCharBeforeIndex).Character);
        int lastValidIndex = validCharBeforeIndex;
        for(int i = validCharBeforeIndex; i >= 0; i--)
        {
            if(!GetCharacterRenderInfo(i).Valid)
                continue;
            
            characterType curCharType = getCharType(GetInternalCharacterDetail(i).Character);

            //Return anything 
            if(lastCharType != curCharType && lastCharType != characterType::SPACE_TAB_NEWLINE)
                return lastValidIndex;

            lastValidIndex = i;
            lastCharType = curCharType;
        }

        return lastValidIndex;
    }

    int TextField::GetEndOfNextWord(int curIndex)
    {
        if(curIndex >= GetLastValidCharacterIndex())
            return CharactersRenderInfos.size();
        
        enum class characterType
        {
            LETTERS,
            NUM,
            SYMBOL,
            SPACE_TAB_NEWLINE,
        };

        auto getCharType = [&](wchar_t c)->characterType
        {
            uint32_t charInt = (uint32_t)c;
            
            //If letters
            if((charInt >= 65 && charInt <= 90) || (charInt >= 97 && charInt <= 122))
                return characterType::LETTERS;
            //If numbers
            if(charInt >= 48 && charInt <= 57)
                return characterType::NUM;
            //If Symbol
            if((charInt >= 33 && charInt <= 47) || (charInt >= 58 && charInt <= 64) || (charInt >= 91 && charInt <= 96) || (charInt >= 123 && charInt <= 126))
                return characterType::SYMBOL;
            //If space, tab or newline 
            if(charInt == 9 || charInt == 10 || charInt == 12 || charInt == 13 || charInt == 32)
                return characterType::SPACE_TAB_NEWLINE;

            return characterType::LETTERS;
        };

        characterType lastCharType = getCharType(GetInternalCharacterDetail(curIndex).Character);
        int lastValidIndex = curIndex;
        for(int i = curIndex; i <= CharactersRenderInfos.size(); i++)
        {
            if(i == CharactersRenderInfos.size())
                return i;
            
            if(!GetCharacterRenderInfo(i).Valid)
                continue;
            
            characterType curCharType = getCharType(GetInternalCharacterDetail(i).Character);
            lastValidIndex = i;

            //Return anything 
            if(lastCharType != curCharType && lastCharType != characterType::SPACE_TAB_NEWLINE)
                return lastValidIndex;

            lastCharType = curCharType;
        }

        return lastValidIndex;
    }

    void TextField::GetCurrentLineStartEndIndex(int curIndex, int& startIndex, int& endIndexInclusive)
    {
        curIndex = curIndex == CharactersRenderInfos.size() ? CharactersRenderInfos.size() - 1 : curIndex;
        for(int i = curIndex; i >= 0; i--)
        {
            if(!GetCharacterRenderInfo(i).Valid)
                continue;
            
            if(GetCharacterRenderInfo(i).CharacterAtNewline)
            {
                startIndex = i;
                break;
            }
        }

        int lastValidIndex = curIndex;
        for(int i = curIndex+1; i < CharactersRenderInfos.size(); i++)
        {
            if(!GetCharacterRenderInfo(i).Valid)
                continue;
            
            if(GetCharacterRenderInfo(i).CharacterAtNewline)
                break;
            
            lastValidIndex = i;
        }
        endIndexInclusive = lastValidIndex;
    }

    void TextField::GetPreviousLineStartEndIndex(int curIndex, int& startIndex, int& endIndexInclusive)
    {
        bool passedCurLine = false;
        bool endIndexSet = false;
        curIndex = curIndex == CharactersRenderInfos.size() ? CharactersRenderInfos.size() - 1 : curIndex;
        for(int i = curIndex; i >= 0; i--)
        {
            if(!GetCharacterRenderInfo(i).Valid)
                continue;
            
            if(passedCurLine && !endIndexSet)
            {
                endIndexSet = true;
                endIndexInclusive = i;
            }

            if(GetCharacterRenderInfo(i).CharacterAtNewline)
            {
                if(!passedCurLine)
                {
                    startIndex = i;
                    endIndexInclusive = i;
                    passedCurLine = true;
                }
                else
                {
                    startIndex = i;
                    break;
                }
            }
        }
    }

    void TextField::GetNextLineStartEndIndex(int curIndex, int& startIndex, int& endIndexInclusive)
    {
        bool startIndexSet = false;
        bool endIndexSet = false;
        int lastValidIndex = curIndex;
        curIndex = curIndex == CharactersRenderInfos.size() ? CharactersRenderInfos.size() - 1 : curIndex;
        for(int i = curIndex+1; i < CharactersRenderInfos.size(); i++)
        {
            if(!GetCharacterRenderInfo(i).Valid)
                continue;
            
            if(GetCharacterRenderInfo(i).CharacterAtNewline)
            {
                if(!startIndexSet)
                {
                    startIndex = i;
                    startIndexSet = true;
                }
                else
                {
                    endIndexInclusive = lastValidIndex;
                    endIndexSet = true;
                    break;
                }
            }
            lastValidIndex = i;
        }

        if(!startIndexSet)
        {
            startIndex = lastValidIndex;
            endIndexInclusive = lastValidIndex;
        }
        else if(!endIndexSet)
            endIndexInclusive = lastValidIndex;
    }

    int TextField::GetPositionForPreviousLine(int curIndex)
    {
        int curLinePosition = 0;
        
        int curLineStart = 0;
        int curLineEnd = 0;
        GetCurrentLineStartEndIndex(curIndex, curLineStart, curLineEnd);

        int prevLineStart = 0;
        int prevLineEnd = 0;
        GetPreviousLineStartEndIndex(curIndex, prevLineStart, prevLineEnd);

        //Find out current position from this line (Depending on alignment)
        //Right
        if(GetTextHorizontalAlignment() == ssGUI::Enums::AlignmentHorizontal::RIGHT)
        {
            curLineEnd = curLineEnd == GetLastValidCharacterIndex() ? curLineEnd + 1 : curLineEnd;
            curLinePosition = curLineEnd - curIndex;
        }
        //Left and center
        else
            curLinePosition = curIndex - curLineStart;
        
        //If there's no previous line, just return start index
        if(prevLineEnd >= curLineStart)
            return curLineStart;

        //Go to previous line if there's any
        if(GetTextHorizontalAlignment() == ssGUI::Enums::AlignmentHorizontal::RIGHT)
        {
            if(curLinePosition > prevLineEnd - prevLineStart)
                return prevLineStart;
            else
                return prevLineEnd - curLinePosition;
        }
        else
        {
            if(curLinePosition > prevLineEnd - prevLineStart)
                return prevLineEnd;
            else
                return prevLineStart + curLinePosition;
        }
        
        return -1;  //Just to suppress return-type warning from GCC
    }

    int TextField::GetPositionForNextLine(int curIndex)
    {
        int curLinePosition = 0;
        
        int curLineStart = 0;
        int curLineEnd = 0;
        GetCurrentLineStartEndIndex(curIndex, curLineStart, curLineEnd);

        int nextLineStart = 0;
        int nextLineEnd = 0;
        GetNextLineStartEndIndex(curIndex, nextLineStart, nextLineEnd);

        //Find out current position from this line (Depending on alignment)
        //Right
        if(GetTextHorizontalAlignment() == ssGUI::Enums::AlignmentHorizontal::RIGHT)
            curLinePosition = curLineEnd - curIndex;
        //Left and center
        else
            curLinePosition = curIndex - curLineStart;

        //If there's no next line, just return end index
        if(curLineEnd >= nextLineStart)
        {
            if(curLineEnd != GetLastValidCharacterIndex())
                return curLineEnd;
            else
                return curLineEnd + 1;
        }
        
        //Go to next line if there's any
        if(GetTextHorizontalAlignment() == ssGUI::Enums::AlignmentHorizontal::RIGHT)
        {
            if(curLinePosition > nextLineEnd - nextLineStart)
                return nextLineStart;
            else
            {
                nextLineEnd = nextLineEnd == GetLastValidCharacterIndex() ? nextLineEnd + 1 : nextLineEnd;
                return nextLineEnd - curLinePosition;
            }
        }
        else
        {
            if(curLinePosition > nextLineEnd - nextLineStart)
            {
                if(nextLineEnd != GetLastValidCharacterIndex())
                    return nextLineEnd;
                else
                    return nextLineEnd + 1;
            }
            else
                return nextLineStart + curLinePosition;
        }
        
        return -1;  //Just to suppress return-type warning from GCC
    }

    void TextField::TextInputUpdate(std::wstring& textInput, bool& refreshBlinkTimer, bool& wordMode)
    {
        int insertIndex = GetStartSelectionIndex() > GetEndSelectionIndex() ? GetStartSelectionIndex() : GetEndSelectionIndex();
        insertIndex = (insertIndex >= 0 && insertIndex <= GetText().size()) ? insertIndex : 0;

        ssGUI::CharacterDetails baseCD;
        if(insertIndex - 1 >= 0)
            baseCD = GetCharacterDetails(insertIndex - 1);
        else if(insertIndex + 1 < GetCharactersDetailsCount())
            baseCD = GetCharacterDetails(insertIndex + 1);
        else
        {
            baseCD.CharacterColor = GetNewTextColor();
            baseCD.DefaultFontIndex = GetFontsCount() == 0 ? 0 : -1;
            baseCD.FontIndex = GetFontsCount() == 0 ? -1 : 0;
            baseCD.FontSize = GetNewTextFontSize();
            baseCD.Underlined = IsNewTextUnderlined();
        }

        int charCount = 0;
        bool alterText = false;

        //Add input characters
        for(int i = 0; i < textInput.size(); i++)
        {
            auto curChar = (int32_t)textInput.at(i);

            //Non control characters
            if((curChar > 31 && curChar < 127) || curChar > 159)
            {
                ssGUI::CharacterDetails cd = baseCD;
                cd.Character = textInput.at(i);

                AddCharacterDetails(insertIndex, cd);
                insertIndex++;
                charCount++;
                alterText = true;
            }
            //Control characters
            else
            {
                switch(curChar) 
                {
                    case 8:     //Backspace
                        if(insertIndex > 0 && GetStartSelectionIndex() == GetEndSelectionIndex() && GetStartSelectionIndex() >= 0)
                        {
                            if(wordMode)
                            {
                                int indexOfPrevWord = GetEndOfPreviousWord(insertIndex);
                                RemoveCharacterDetails(indexOfPrevWord, insertIndex);
                                charCount -= insertIndex - indexOfPrevWord;
                                insertIndex -= insertIndex - indexOfPrevWord;
                            }
                            else
                            {
                                RemoveCharacterDetails(--insertIndex);
                                charCount--;
                            }
                        }
                        alterText = true;
                        break;
                    case 9:     //Tab
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\t';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 10:    //Newline or Line Feed
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\n';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 12:    //Form Feed (Ctrl Enter if supported)
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\n';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 13:    //Carriage Return (Only this character is inserted when Enter is pressed, at least on Linux for whatever reason)
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\n';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 27:    //Escape
                        SetStartSelectionIndex(GetEndSelectionIndex());
                        break;
                    case 127:   //Delete
                        if(insertIndex < GetCharactersDetailsCount() && GetStartSelectionIndex() == GetEndSelectionIndex() && 
                            GetStartSelectionIndex() >= 0)
                        {
                            if(wordMode)
                            {
                                int indexOfNextWord = GetEndOfNextWord(insertIndex);
                                RemoveCharacterDetails(insertIndex, indexOfNextWord);
                            }
                            else
                            {
                                RemoveCharacterDetails(insertIndex);
                            }
                        }
                        
                        alterText = true;
                        break;
                    case 133:   //Next line
                        //TODO: Maybe support this? Go to next line
                        break;
                }
            }
        }

        //If the text is changed, remove any hightlighted text and move caret accordingly
        if(alterText)
        {
            //Removes the text if there's any highlighted
            if(GetStartSelectionIndex() != GetEndSelectionIndex())
            {                
                if(GetStartSelectionIndex() < GetEndSelectionIndex())
                {
                    RemoveCharacterDetails(GetStartSelectionIndex(), GetEndSelectionIndex());
                    SetEndSelectionIndex(GetStartSelectionIndex());
                }
                else
                {
                    RemoveCharacterDetails(GetEndSelectionIndex(), GetStartSelectionIndex());
                    SetStartSelectionIndex(GetEndSelectionIndex());
                }
            }

            if(GetStartSelectionIndex() == -1)
            {
                SetStartSelectionIndex(0);
                SetEndSelectionIndex(0);
            }
            SetEndSelectionIndex(GetEndSelectionIndex() + charCount);
            SetStartSelectionIndex(GetEndSelectionIndex());
            refreshBlinkTimer = true;
        }

        RedrawObject();
        RecalculateTextNeeded = true;
    }

    void TextField::CaretNavigationUpdate(ssGUI::Backend::BackendSystemInputInterface* inputInterface, bool& refreshBlinkTimer, bool& blockKeys, bool& wordMode)
    {
        //Holding shift to enable selection mode
        bool selectionMode =    inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT_SHIFT) || 
                                inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT_SHIFT);
        
        //Arrow navigation
        if(GetStartSelectionIndex() >= 0 && GetEndSelectionIndex() >= 0)
        {
            if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT))
            {
                auto moveLeft = [&]()
                {
                    if(GetEndSelectionIndex() > GetFirstValidCharacterIndex())
                    {
                        if(!wordMode)
                        {
                            do
                            {
                                SetEndSelectionIndex(GetEndSelectionIndex() - 1);
                            }
                            while(!GetCharacterRenderInfo(GetEndSelectionIndex()).Valid);
                        }
                        else
                            SetEndSelectionIndex(GetEndOfPreviousWord(GetEndSelectionIndex()));
                        
                        if(!selectionMode)
                            SetStartSelectionIndex(GetEndSelectionIndex());   
                    }
                };

                //When the user first press the left arrow key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::LEFT))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveLeft();
                }
                //When the user is holding down the left arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveLeft();
                }

                refreshBlinkTimer = true;
                blockKeys = true;
            }
            else if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT))
            {
                auto moveRight = [&]()
                {
                    if(GetEndSelectionIndex() + 1 <= GetCharactersDetailsCount())
                    {
                        if(!wordMode)
                        {
                            do
                            {
                                SetEndSelectionIndex(GetEndSelectionIndex() + 1);

                                if(GetEndSelectionIndex() >= GetCharactersDetailsCount())
                                    break;
                            }
                            while(!GetCharacterRenderInfo(GetEndSelectionIndex()).Valid);
                        }
                        else
                            SetEndSelectionIndex(GetEndOfNextWord(GetEndSelectionIndex()));
                    
                        if(!selectionMode)
                            SetStartSelectionIndex(GetEndSelectionIndex());
                    }
                };

                //When the user first press the right arrow key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::RIGHT))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveRight();
                }
                //When the user is holding down the right arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveRight();
                }
                
                refreshBlinkTimer = true;
                blockKeys = true;
            }
            
            if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::UP))
            {
                auto moveUp = [&]()
                {
                    if(!selectionMode)
                    {
                        SetEndSelectionIndex(GetPositionForPreviousLine(GetEndSelectionIndex()));
                        SetStartSelectionIndex(GetEndSelectionIndex());
                    }
                    else
                    {
                        SetEndSelectionIndex(GetPositionForPreviousLine(GetEndSelectionIndex()));
                    }
                };

                //When the user first press the up arrow key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::UP))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveUp();
                }
                //When the user is holding down the up arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveUp();
                }
                
                refreshBlinkTimer = true;
                blockKeys = true;
            }
            else if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::DOWN))
            {
                auto moveDown = [&]()
                {
                    if(!selectionMode)
                    {
                        SetEndSelectionIndex(GetPositionForNextLine(GetEndSelectionIndex()));
                        SetStartSelectionIndex(GetEndSelectionIndex());
                    }
                    else
                    {
                        SetEndSelectionIndex(GetPositionForNextLine(GetEndSelectionIndex()));
                    }
                };

                //When the user first press the right down key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::DOWN))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveDown();
                }
                //When the user is holding down the down arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveDown();
                }
                
                refreshBlinkTimer = true;
                blockKeys = true;
            }
        }
    }
    
    //You only need to override this when you are rendering anything.
    void TextField::ConstructRenderInfo()
    {
        ssLOG_FUNC_ENTRY();
        ssGUI::Text::ConstructRenderInfo();

        int lastValidIndex = GetLastValidCharacterIndex();
        glm::vec2 drawPos = GetGlobalPosition();
        float height = 0;
        float caretWidth = GetEndSelectionIndex() < 0 || GetEndSelectionIndex() >= GetCharactersDetailsCount() ? 
            GetNewTextFontSize() / 15.f : 
            GetCharacterDetails(GetEndSelectionIndex()).FontSize / 15.f;

        if(GetEndSelectionIndex() >= 0 && lastValidIndex >= 0)
        {
            if(GetEndSelectionIndex() > lastValidIndex)
            {
                //Last character
                if(GetCharacterDetails(lastValidIndex).Character != '\n')
                {
                    drawPos += GetCharacterRenderInfo(lastValidIndex).BaselinePosition + 
                        glm::vec2(GetCharacterRenderInfo(lastValidIndex).Advance, 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY);
                    
                    height = GetCharacterRenderInfo(lastValidIndex).LineMaxY - 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY;
                }
                //If caret is pointing Newline, go to next line
                if(GetCharacterDetails(lastValidIndex).Character == '\n')
                {
                    const ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(lastValidIndex);
                    ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
                    if(curDetail.FontIndex != -1)
                        fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
                    else
                        fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
                    
                    auto horiAlignment = GetTextHorizontalAlignment(); 
                    if(horiAlignment == ssGUI::Enums::AlignmentHorizontal::LEFT)
                    {
                        drawPos.x += GetTextHorizontalPadding();
                    }
                    else if(horiAlignment == ssGUI::Enums::AlignmentHorizontal::CENTER)
                    {
                        drawPos.x += GetSize().x / 2;
                    }
                    else
                    {
                        drawPos.x += GetSize().x - GetTextHorizontalPadding();
                    }

                    drawPos.y += GetCharacterRenderInfo(lastValidIndex).BaselinePosition.y + 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY +
                        fontInterface->GetLineSpacing(curDetail.FontSize) + GetLineSpace();
                    
                    height = GetCharacterRenderInfo(lastValidIndex).LineMaxY - 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY;
                }
            }
            else
            {
                drawPos += GetCharacterRenderInfo(GetEndSelectionIndex()).BaselinePosition + 
                    glm::vec2(0, GetCharacterRenderInfo(GetEndSelectionIndex()).LineMinY);
                
                height = GetCharacterRenderInfo(GetEndSelectionIndex()).LineMaxY - 
                    GetCharacterRenderInfo(GetEndSelectionIndex()).LineMinY;
            }

            //Draw caret
            if(BlinkCaret || !IsInteractable() || !IsFocused())
            {
                ssLOG_FUNC_EXIT();
                return;
            }

            ssGUI::DrawingEntity caretEntity;
            if(GetEndSelectionIndex() > GetStartSelectionIndex())
            {
                caretEntity.Vertices.push_back(drawPos);
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, 0));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(0, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));
            }
            else
            {
                caretEntity.Vertices.push_back(drawPos + glm::vec2(-caretWidth, 0));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos);
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(0, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(-caretWidth, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));
            }
            caretEntity.EntityName = TEXTFIELD_CARET_SHAPE_NAME;
            DrawingEntities.push_back(caretEntity);
        }
        else
        {
            //Draw caret
            if(BlinkCaret || !IsInteractable() || !IsFocused())
            {
                ssLOG_FUNC_EXIT();
                return;
            }

            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
            if(GetFontsCount() > 0)
                fontInterface = GetFont(0)->GetBackendFontInterface();
            else if(GetDefaultFontsCount() > 0)
                fontInterface = GetDefaultFont(0)->GetBackendFontInterface();
            else
            {
                ssLOG_FUNC_EXIT();
                return;
            }

            height = fontInterface->GetLineSpacing(GetNewTextFontSize()) + GetLineSpace();
            
            switch(GetTextHorizontalAlignment()) 
            {
                case ssGUI::Enums::AlignmentHorizontal::LEFT:
                    drawPos.x += GetTextHorizontalPadding();
                    break;
                case ssGUI::Enums::AlignmentHorizontal::CENTER:
                    drawPos.x += GetSize().x / 2;
                    break;
                case ssGUI::Enums::AlignmentHorizontal::RIGHT:
                    drawPos.x += GetSize().x - GetTextHorizontalPadding();
                    break;
            }

            switch(GetTextVerticalAlignment()) 
            {
                case ssGUI::Enums::AlignmentVertical::TOP:
                    drawPos.y += GetTextVerticalPadding();
                    break;
                case ssGUI::Enums::AlignmentVertical::CENTER:
                    drawPos.y += GetSize().y / 2 - height / 2;
                    break;
                case ssGUI::Enums::AlignmentVertical::BOTTOM:
                    drawPos.y += GetSize().y - GetNewTextFontSize() - GetTextVerticalPadding();
                    break;
            }

            ssGUI::DrawingEntity caretEntity;

            caretEntity.Vertices.push_back(drawPos);
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

            caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, 0));
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

            caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, height));
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

            caretEntity.Vertices.push_back(drawPos + glm::vec2(0, height));
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));
            caretEntity.EntityName = TEXTFIELD_CARET_SHAPE_NAME;
            DrawingEntities.push_back(caretEntity);
        }

        ssLOG_FUNC_EXIT();
    }
    
    void TextField::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                ssGUI::GUIObject* mainWindow)
    {       
        bool blockKeys = false;
        bool refreshBlinkTimer = false;
        bool wordMode = false;
        if(IsInteractable() && IsBlockInput())
        {
            std::wstring textInput = inputInterface->GetTextInput();

            //Holding ctrl to enable word mode
            wordMode =  inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT_CTRL) || 
                        inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT_CTRL);

            //Pasting
            if( inputStatus.KeyInputBlockedObject == nullptr && 
                inputInterface->ClipbaordHasText() && wordMode && 
                inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::V) &&
                !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::LetterKey::V))
            {
                std::wstring clipboardStr;
                inputInterface->GetClipboardText(clipboardStr);
                textInput += clipboardStr;
            }

            //Text input
            //TODO: Put textInput to a CharacterDetails vector and insert the whole vector instead of 1 by 1
            if(inputStatus.KeyInputBlockedObject == nullptr && IsFocused() && !textInput.empty())
            {
                blockKeys = true;
                TextInputUpdate(textInput, refreshBlinkTimer, wordMode);
            }
        }

        ssGUI::Text::MainLogic(inputInterface, inputStatus, mainWindow);
        
        if(IsInteractable())
        {
            //Caret navigation
            if(inputStatus.KeyInputBlockedObject == nullptr)
                CaretNavigationUpdate(inputInterface, refreshBlinkTimer, blockKeys, wordMode);

            //Blinking caret
            if(refreshBlinkTimer)
                LastBlinkTime = inputInterface->GetElapsedTime() - BlinkDuration;

            uint64_t blinkDiff = inputInterface->GetElapsedTime() - LastBlinkTime;
            bool curState = BlinkCaret;
            if(blinkDiff < BlinkDuration)
                BlinkCaret = true;
            else if(blinkDiff < BlinkDuration * 2)
                BlinkCaret = false;
            else
            {
                LastBlinkTime = inputInterface->GetElapsedTime();
                BlinkCaret = true;
            }

            if(BlinkCaret != curState)
                RedrawObject();

            if(blockKeys)
                inputStatus.KeyInputBlockedObject = this;
        }
    }

    const std::string TextField::TEXTFIELD_CARET_SHAPE_NAME = "Text Caret";

    TextField::TextField() :    LastBlinkTime(0),
                                BlinkDuration(500),
                                BlinkCaret(false),
                                LastArrowNavStartTime(0),
                                ArrowNavPauseDuration(500),
                                LastArrowNavTime(0),
                                ArrowNavInterval(20)
    {
        SetBlockInput(true);
        SetMinSize(glm::vec2(35, 35));
        SetTextAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT, ssGUI::Enums::AlignmentVertical::TOP);
        
        SetBackgroundColor(glm::ivec4(127, 127, 127, 255));

        AddExtension<ssGUI::Extensions::RoundedCorners>();
        
        AddExtension<ssGUI::Extensions::Outline>()->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        
        //Add shadow to window
        auto shadowEx = AddExtension<ssGUI::Extensions::BoxShadow>();
        shadowEx->SetBlurRadius(20);
        shadowEx->SetSizeOffset(glm::vec2(10, 10));
    }

    TextField::~TextField()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    ssGUI::Enums::GUIObjectType TextField::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::TEXT | ssGUI::Enums::GUIObjectType::TEXT_FIELD;
    }

    TextField* TextField::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        TextField* temp = new TextField(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                ssLOG_FUNC_EXIT();
                return nullptr;
            }
        }

        ssLOG_FUNC_EXIT();
        return temp;
    }
}