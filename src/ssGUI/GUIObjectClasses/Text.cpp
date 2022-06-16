#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "glm/gtx/norm.hpp"
#include <cmath>


namespace ssGUI
{    
    //TODO: Encapsulate it in class for deallocation
    std::vector<ssGUI::Font*> Text::DefaultFonts = []()->std::vector<ssGUI::Font*>
    {
        FUNC_DEBUG_ENTRY("LoadDefaultFont");

        std::vector<ssGUI::Font*> defaultFonts;

        auto font = new ssGUI::Font();
        if(!font->GetBackendFontInterface()->LoadFromPath("Resources/NotoSans-Regular.ttf"))
        {
            DEBUG_LINE("Failed to load default font");
            delete font;
            FUNC_DEBUG_EXIT("LoadDefaultFont");
            return defaultFonts;
        }
        else
        {
            defaultFonts.push_back(font);
            FUNC_DEBUG_EXIT("LoadDefaultFont");
            return defaultFonts;
        }
        FUNC_DEBUG_EXIT("LoadDefaultFont");
    }();    //Brackets at the end to call this lambda, pretty cool.
    
    Text::Text(Text const& other) : Widget(other)
    {
        CurrentText = other.GetText();
        RecalculateTextNeeded = true;
        OverrideCharactersDetails = other.OverrideCharactersDetails;
        CharactersRenderInfos = other.CharactersRenderInfos;
        CurrentCharacterDetails = other.CurrentCharacterDetails;
        Overflow = other.Overflow;
        FontSize = other.GetFontSize();
        TextColor = other.GetTextColor();
        TextUnderline = other.IsTextUnderlined();
        MultilineAllowed = other.IsMultilineAllowed();
        WrappingMode = other.GetWrappingMode();
        HorizontalAlignment = other.GetHorizontalAlignment();
        VerticalAlignment = other.GetVerticalAlignment();
        CurrentFonts = other.CurrentFonts;
        HorizontalPadding = other.GetHorizontalPadding();
        VerticalPadding = other.GetVerticalPadding();
        CharacterSpace = other.GetCharacterSpace();
        LineSpace = other.GetLineSpace();
        TabSize = other.GetTabSize();
        LastDefaultFonts = other.LastDefaultFonts;
    }

    void Text::ConstructCharacterDetails()
    {
        CurrentCharacterDetails.clear();
        
        //Using override character details 
        if(!OverrideCharactersDetails.empty())
        {
            CurrentCharacterDetails = OverrideCharactersDetails;
        }
        //Using normal text
        else
        {
            for(int i = 0; i < CurrentText.size(); i++)
            {
                ssGUI::CharacterDetails detail;
                detail.Character = CurrentText[i];
                detail.FontSize = GetFontSize();
                detail.CharacterColor = GetTextColor();
                detail.Underlined = IsTextUnderlined();
                CurrentCharacterDetails.push_back(detail);
            }
        }
    }

    void Text::AssignSupportedFont()
    {
        FUNC_DEBUG_ENTRY();
        
        for(int i = 0; i < CurrentCharacterDetails.size(); i++)
        {
            ssGUI::CharacterDetails& detail = CurrentCharacterDetails[i];
            
            if(detail.Character == L'\0')
                continue;
            
            //Assign font index if not assigned
            if(detail.FontIndex == -1 && detail.DefaultFontIndex == -1)
            {
                if(GetFontsCount() > 0)
                    detail.FontIndex = 0;
                else
                    detail.DefaultFontIndex = 0;
            }
            
            //Just move on if it is newline, tab or space
            if(detail.Character == L'\n' || detail.Character == L'\t' || detail.Character == L' ')
                continue;

            //Check each font if character is supported. If not, fallback to default fonts
            bool fallbackToDefault = false;
            if(detail.FontIndex != -1)
            {
                while (!GetFont(detail.FontIndex)->IsCharacterSupported(detail.Character))
                {                   
                    if(detail.FontIndex == GetFontsCount() -1)
                    {
                        fallbackToDefault = true;
                        break;
                    }

                    detail.FontIndex++;
                }
            }
            
            //Default fonts
            if(fallbackToDefault || (detail.FontIndex == -1 && detail.DefaultFontIndex != -1))
            {
                if(GetDefaultFontsCount() == 0)
                {
                    detail.DefaultFontIndex = -1;
                    continue;
                }

                while (!GetDefaultFont(detail.DefaultFontIndex)->IsCharacterSupported(detail.Character))
                {
                    if(detail.DefaultFontIndex == GetDefaultFontsCount() -1)
                        break;

                    detail.DefaultFontIndex++;
                }
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::DrawCharacter(glm::vec2 positionOffset, ssGUI::CharacterRenderInfo info, ssGUI::CharacterDetails details)
    {
        if(!info.Rendered)
            return;

        glm::vec2 position = positionOffset + info.RenderPosition;
        ssGUI::Font* targetFont = nullptr;
        
        if(details.FontIndex != -1)
            targetFont = GetFont(details.FontIndex);
        else if(details.DefaultFontIndex != -1)
            targetFont = GetDefaultFont(details.DefaultFontIndex);

        if(targetFont == nullptr)
            return;
        
        DrawingVerticies.push_back(position                                 + info.DrawOffset);
        DrawingVerticies.push_back(position + glm::vec2(info.Size.x, 0)     + info.DrawOffset);
        DrawingVerticies.push_back(position + info.Size                     + info.DrawOffset);
        DrawingVerticies.push_back(position + glm::vec2(0, info.Size.y)     + info.DrawOffset);

        DrawingColours.push_back(details.CharacterColor);
        DrawingColours.push_back(details.CharacterColor);
        DrawingColours.push_back(details.CharacterColor);
        DrawingColours.push_back(details.CharacterColor);

        DrawingUVs.push_back(info.UVOrigin);
        DrawingUVs.push_back(info.UVOrigin + glm::vec2(info.Size.x, 0));
        DrawingUVs.push_back(info.UVOrigin + info.Size);
        DrawingUVs.push_back(info.UVOrigin + glm::vec2(0, info.Size.y));

        DrawingCounts.push_back(4);
        ssGUI::DrawingProperty currentProperty;
        currentProperty.fontP = targetFont->GetBackendFontInterface();
        currentProperty.characterSize = details.FontSize;

        DrawingProperties.push_back(currentProperty);
    }

    void Text::FormatNewlinesCharacters()
    {
        FUNC_DEBUG_ENTRY();

        //If multiline is disabled, replace all newline to spaces and remove all \r
        if(!MultilineAllowed)
        {
            for(int i = 0; i < CurrentCharacterDetails.size(); i++)
            {
                if(CurrentCharacterDetails.at(i).Character == L'\n')
                    CurrentCharacterDetails.at(i).Character = L' ';
                else if(CurrentCharacterDetails.at(i).Character == L'\r')
                    CurrentCharacterDetails.at(i).Character = L'\0';
            }
        }
        //Otherwise find out how many lines and length and remove all \r
        else
        {
            for(int i = 0; i < CurrentCharacterDetails.size(); i++)
            {
                if(CurrentCharacterDetails.at(i).Character == L'\r')
                    CurrentCharacterDetails.at(i).Character = L'\0';
            }
        }

        FUNC_DEBUG_EXIT();
    }

    float Text::GetClosestTabSpace(float startPos, float tabSpace, float targetPos)
    {
        float tabAmount = (targetPos - startPos) / tabSpace;

        //If at tab
        //https://stackoverflow.com/questions/5796983/checking-if-float-is-an-integer#answer-25274904
        if(fabs(round(tabAmount) - tabAmount) <= 0.00001f)
            return startPos + (round(tabAmount) + 1) * tabSpace;
        else
            return startPos + ceil(tabAmount) * tabSpace;
    }

    void Text::ConstructRenderInfosForWordWrapping()
    {
        FUNC_DEBUG_ENTRY();
        
        int currentWordIndex = 0;
        float currentLineLength = GetHorizontalPadding();
        float currentWordLength = 0;
        float drawXPos = GetHorizontalPadding();

        wchar_t prevChar = 0;
        Overflow = false;

        for (int i = 0; i < CurrentCharacterDetails.size(); i++)
        {
            CharacterDetails curDetail = CurrentCharacterDetails.at(i);
            wchar_t curChar = curDetail.Character;

            //If this character is null or fonts are not available, skip
            if(curChar == L'\0' ||
                (curDetail.FontIndex == -1 && curDetail.DefaultFontIndex == -1) ||
                (curDetail.FontIndex != -1 && GetFont(curDetail.FontIndex) == nullptr) ||
                (curDetail.DefaultFontIndex != -1 && GetDefaultFont(curDetail.DefaultFontIndex) == nullptr))
            {
                prevChar = 0;
                CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                continue;
            }
            
            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
            if(curDetail.FontIndex != -1)
                fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
            
            // Apply the kerning offset
            drawXPos += fontInterface->GetKerning(prevChar, curChar, curDetail.FontSize);

            prevChar = curChar;
            
            //See if the current word exceeds widget width when a word finishes
            if ((curChar == L' ') || (curChar == L'\n') || (curChar == L'\t') || i == CurrentCharacterDetails.size() - 1)
            {
                //last character
                if(i == CurrentCharacterDetails.size() - 1 && curChar != L' ' && curChar != L'\n' && curChar != L'\t')
                {
                    ssGUI::CharacterRenderInfo info = fontInterface->GetCharacterRenderInfo(curChar, curDetail.FontSize);
                    float characterLength = info.Advance;

                    CharactersRenderInfos[i] = info;
                    CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);

                    currentWordLength += characterLength;
                }
                
                prevChar = 0;
                
                //Check if word width is wider than widget width by itself, it will be on its own line
                if(currentWordLength - GetCharacterSpace() + GetHorizontalPadding() * 2 > GetSize().x && currentLineLength == GetHorizontalPadding())
                {
                    currentWordIndex = i + 1;
                    currentWordLength = 0;
                    CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                    // CharactersRenderInfos[i].Rendered = true;
                    
                    drawXPos = GetHorizontalPadding();
                    Overflow = true;
                }
                //Otherwise check if adding current word length to current line length exceeds widget width
                //If so, reset i and current line length and current word length and go to next line
                else if(currentWordLength - GetCharacterSpace() + currentLineLength + GetHorizontalPadding() > GetSize().x)
                {
                    i = currentWordIndex - 1;
                    currentLineLength = GetHorizontalPadding();
                    currentWordLength = 0;

                    drawXPos = GetHorizontalPadding();
                }
                //Otherwise, set word index to next character and reset current word length.
                //Assign render position to space, tab and newline.
                //Assign advance to space and tab.
                else 
                {
                    currentWordIndex = i + 1;
                    currentLineLength += currentWordLength;
                    currentWordLength = 0;
                    CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                    // CharactersRenderInfos[i].Rendered = true;
                    float whitespaceWidth = fontInterface->GetCharacterRenderInfo(L' ', curDetail.FontSize).Advance + GetCharacterSpace();

                    switch (curChar)
                    {
                        case L' ':
                            CharactersRenderInfos[i].Advance = whitespaceWidth; 
                            drawXPos += whitespaceWidth;
                            currentLineLength += whitespaceWidth;
                            break;
                        case L'\t': 
                        {
                            float newPos = GetClosestTabSpace(GetHorizontalPadding(), whitespaceWidth * GetTabSize(), drawXPos);
                            float actualTabSpace = newPos - drawXPos;
                            CharactersRenderInfos[i].Advance = actualTabSpace; 
                            drawXPos += actualTabSpace;
                            currentLineLength += actualTabSpace;
                            break;
                        }
                        case L'\n': 
                            drawXPos = GetHorizontalPadding();
                            currentLineLength = GetHorizontalPadding();
                            break;
                    }
                }
            }
            //Otherwise, add the current character
            else
            {
                ssGUI::CharacterRenderInfo info = fontInterface->GetCharacterRenderInfo(curChar, curDetail.FontSize);
                float characterLength = info.Advance;

                CharactersRenderInfos[i] = info;
                CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);

                if(currentLineLength == GetHorizontalPadding() && currentWordLength == 0)
                    CharactersRenderInfos[i].CharacterAtNewline = true;

                currentWordLength += characterLength + GetCharacterSpace();
                drawXPos += characterLength + GetCharacterSpace();
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ConstructRenderInfosForCharacterWrapping()
    {
        FUNC_DEBUG_ENTRY();

        float currentLineLength = GetHorizontalPadding();
        // glm::vec2 currentDrawPos = glm::vec2();
        float drawXPos = GetHorizontalPadding();

        wchar_t prevChar = 0;
        Overflow = false;

        for (int i = 0; i < CurrentCharacterDetails.size(); i++)
        {
            CharacterDetails curDetail = CurrentCharacterDetails.at(i);
            wchar_t curChar = curDetail.Character;

            if(curChar == L'\0' ||
                (curDetail.FontIndex == -1 && curDetail.DefaultFontIndex == -1) ||
                (curDetail.FontIndex != -1 && GetFont(curDetail.FontIndex) == nullptr) ||
                (curDetail.DefaultFontIndex != -1 && GetDefaultFont(curDetail.DefaultFontIndex) == nullptr))
            {
                prevChar = 0;
                CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                continue;
            }
            
            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
            if(curDetail.FontIndex != -1)
                fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
            
            // Apply the kerning offset
            drawXPos += fontInterface->GetKerning(prevChar, curChar, curDetail.FontSize) /* GetCharacterSpace()*/;

            prevChar = curChar;

            //If space or tab or newline character, just append
            if (curChar == L' ' || curChar == L'\n' || curChar == L'\t')
            {
                CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                // CharactersRenderInfos[i].Rendered = true;
                float whitespaceWidth = fontInterface->GetCharacterRenderInfo(L' ', curDetail.FontSize).Advance + GetCharacterSpace();

                switch (curChar)
                {
                    case L' ':  
                        drawXPos += whitespaceWidth;
                        currentLineLength += whitespaceWidth;
                        CharactersRenderInfos[i].Advance = whitespaceWidth;    
                        break;
                    case L'\t': 
                    {
                        float newPos = GetClosestTabSpace(GetHorizontalPadding(), whitespaceWidth * GetTabSize(), drawXPos);
                        float actualTabSpace = newPos - drawXPos;
                        drawXPos += actualTabSpace;
                        currentLineLength += actualTabSpace;
                        CharactersRenderInfos[i].Advance = actualTabSpace; 
                        break;
                    }
                    case L'\n': 
                        drawXPos = GetHorizontalPadding();
                        currentLineLength = GetHorizontalPadding();
                        break;
                }
            }
            else 
            {
                ssGUI::CharacterRenderInfo info = fontInterface->GetCharacterRenderInfo(curChar, curDetail.FontSize);
                float characterLength = info.Advance;

                //If one character is taking up the whole line, reset line length and go to next line
                if(characterLength + GetHorizontalPadding()* 2 > GetSize().x && currentLineLength == 0)
                {
                    CharactersRenderInfos[i] = info;
                    CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                    CharactersRenderInfos[i].CharacterAtNewline = true;

                    currentLineLength = GetHorizontalPadding();
                    Overflow = true;
                    drawXPos = GetHorizontalPadding();
                }
                //If exceed widget width, reset i and line length and go to next line
                else if(currentLineLength + characterLength + GetHorizontalPadding() > GetSize().x)
                {
                    currentLineLength = GetHorizontalPadding();
                    drawXPos = GetHorizontalPadding();
                    i--;
                }
                else
                {
                    CharactersRenderInfos[i] = info;
                    CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                    if(currentLineLength == GetHorizontalPadding())
                        CharactersRenderInfos[i].CharacterAtNewline = true;

                    currentLineLength += characterLength + GetCharacterSpace();
                    drawXPos += characterLength + GetCharacterSpace();
                }
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ConstructRenderInfosForNoWrapping()
    {
        FUNC_DEBUG_ENTRY();
        
        float drawXPos = GetHorizontalPadding();

        wchar_t prevChar = 0;
        bool nextCharOnNewline = true;
        Overflow = false;

        for (int i = 0; i < CurrentCharacterDetails.size(); i++)
        {
            CharacterDetails curDetail = CurrentCharacterDetails.at(i);
            wchar_t curChar = curDetail.Character;

            if(curChar == L'\0' ||
                (curDetail.FontIndex == -1 && curDetail.DefaultFontIndex == -1) ||
                (curDetail.FontIndex != -1 && GetFont(curDetail.FontIndex) == nullptr) ||
                (curDetail.DefaultFontIndex != -1 && GetDefaultFont(curDetail.DefaultFontIndex) == nullptr))
            {
                prevChar = 0;
                CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                continue;
            }
            
            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
            if(curDetail.FontIndex != -1)
                fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
            
            // Apply the kerning offset
            drawXPos += fontInterface->GetKerning(prevChar, curChar, curDetail.FontSize);

            prevChar = curChar;

            //If space or tab or newline character, just append
            if (curChar == L' ' || curChar == L'\n' || curChar == L'\t')
            {
                CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);
                // CharactersRenderInfos[i].Rendered = true;
                float whitespaceWidth = fontInterface->GetCharacterRenderInfo(L' ', curDetail.FontSize).Advance + GetCharacterSpace();

                if(nextCharOnNewline)
                {
                    CharactersRenderInfos[i].CharacterAtNewline = true;
                    nextCharOnNewline = false;
                }

                switch (curChar)
                {
                    case L' ':  
                        drawXPos += whitespaceWidth;
                        CharactersRenderInfos[i].Advance = whitespaceWidth; 
                        break;
                    case L'\t': 
                    {
                        float newPos = GetClosestTabSpace(GetHorizontalPadding(), whitespaceWidth * GetTabSize(), drawXPos);
                        float actualTabSpace = newPos - drawXPos;
                        drawXPos += actualTabSpace;
                        CharactersRenderInfos[i].Advance = actualTabSpace;
                        break;
                    }
                    case L'\n': 
                        nextCharOnNewline = true;
                        drawXPos = GetHorizontalPadding();
                        break;
                }

                if(drawXPos + GetHorizontalPadding() > GetSize().x)
                    Overflow = true;   
            }
            else 
            {
                ssGUI::CharacterRenderInfo info = fontInterface->GetCharacterRenderInfo(curChar, curDetail.FontSize);
                float characterLength = info.Advance;

                CharactersRenderInfos[i] = info;
                CharactersRenderInfos[i].RenderPosition = glm::vec2(drawXPos, 0);

                if(nextCharOnNewline)
                {
                    CharactersRenderInfos[i].CharacterAtNewline = true;
                    nextCharOnNewline = false;
                }

                drawXPos += characterLength + GetCharacterSpace();

                if(drawXPos + GetHorizontalPadding() > GetSize().x)
                    Overflow = true;                    
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ApplyFontLineSpacing()
    {
        FUNC_DEBUG_ENTRY();
        if(CharactersRenderInfos.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        float curMaxFontNewline = 0;
        int currentIndex = 0;
        int currentLineIndex = 0;
        float currentOffset = 0;
        int lineCount = 0;
        ssGUI::Backend::BackendFontInterface* backendFont = nullptr;

        while (currentIndex < CharactersRenderInfos.size())
        {
            //Record max font newline height if needed
            if(CurrentCharacterDetails[currentIndex].FontIndex != -1 && GetFont(CurrentCharacterDetails[currentIndex].FontIndex) != nullptr)
                backendFont = GetFont(CurrentCharacterDetails[currentIndex].FontIndex)->GetBackendFontInterface();
            else if(CurrentCharacterDetails[currentIndex].DefaultFontIndex != -1 && 
                    GetDefaultFont(CurrentCharacterDetails[currentIndex].DefaultFontIndex) != nullptr)
            {
                backendFont = GetDefaultFont(CurrentCharacterDetails[currentIndex].DefaultFontIndex)->GetBackendFontInterface();   
            }
            else
            {
                currentIndex++;
                continue;
            }

            //When there's a newline, offset the current line
            if(CharactersRenderInfos[currentIndex].CharacterAtNewline)
            {
                //First line doesn't use font line space
                if(lineCount == 1)
                {
                    currentOffset += curMaxFontNewline;
                    
                    for(int i = currentLineIndex; i < currentIndex; i++)
                    {
                        CharactersRenderInfos[i].RenderPosition.y += currentOffset;
                        CharactersRenderInfos[i].LineMinY = -curMaxFontNewline;
                    }

                    curMaxFontNewline = 0;
                    currentLineIndex = currentIndex;
                    lineCount++;
                }
                //Second line and beyond uses font line space
                else if(lineCount >= 2)
                {
                    currentOffset += backendFont->GetLineSpacing(curMaxFontNewline) + GetLineSpace();
                    
                    for(int i = currentLineIndex; i < currentIndex; i++)
                    {
                        CharactersRenderInfos[i].RenderPosition.y += currentOffset;
                        CharactersRenderInfos[i].LineMinY = -backendFont->GetLineSpacing(curMaxFontNewline) - GetLineSpace();
                    }

                    curMaxFontNewline = 0;
                    currentLineIndex = currentIndex;
                }
                //First character is always new line, ignore it
                else
                {
                    curMaxFontNewline = 0;
                    currentLineIndex = currentIndex;
                    lineCount++;
                }
            }

            if(CurrentCharacterDetails[currentIndex].FontSize > curMaxFontNewline)
                curMaxFontNewline = CurrentCharacterDetails[currentIndex].FontSize;
            

            //If this is the last character, offset the current line
            if(currentIndex == CharactersRenderInfos.size() - 1 && backendFont != nullptr)
            {
                currentOffset += lineCount == 1 ? curMaxFontNewline : backendFont->GetLineSpacing(curMaxFontNewline) + GetLineSpace();
                
                for(int i = currentLineIndex; i <= currentIndex; i++)
                    CharactersRenderInfos[i].RenderPosition.y += currentOffset;

                //Update vertical overflow
                if(!IsOverflow() && CharactersRenderInfos[currentIndex].RenderPosition.y + GetVerticalPadding() * 2 > GetSize().y)
                    Overflow = true;

                break;
            }
            
            currentIndex++;
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ApplyTextAlignment()
    {
        FUNC_DEBUG_ENTRY();
        
        if(CharactersRenderInfos.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        //Text alignment
        float lineStartPos = 0;
        int lineStartIndex = 0;
        float lineEndPos = lineStartPos;
        float currentLineHeight = -1;

        //For each line, find out how long it is and align accordingly
        for(int i = 0; i < CharactersRenderInfos.size(); i++)
        {
            //If this character is on a newline
            if(CharactersRenderInfos[i].RenderPosition.y != currentLineHeight)
            {
                //Align the previous line first
                if(i > 0)
                {
                    lineEndPos = CharactersRenderInfos[i-1].RenderPosition.x + CharactersRenderInfos[i-1].DrawOffset.x +
                        CharactersRenderInfos[i-1].Size.x + GetHorizontalPadding();
                    float alignOffset = 0; 

                    switch (HorizontalAlignment)
                    {
                        case ssGUI::Enums::TextAlignmentHorizontal::LEFT:
                            break;
                    
                        case ssGUI::Enums::TextAlignmentHorizontal::CENTER:
                            alignOffset = (GetSize().x * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                            break;
                        
                        case ssGUI::Enums::TextAlignmentHorizontal::RIGHT:
                            alignOffset = GetSize().x - lineEndPos;
                            break;
                    }

                    for(int j = lineStartIndex; j < i; j++)
                        CharactersRenderInfos[j].RenderPosition.x += alignOffset;
                }

                //Then record where the newline starts
                lineStartPos = 0;
                currentLineHeight = CharactersRenderInfos[i].RenderPosition.y;
                lineStartIndex = i;
            }
            
            //End of character
            if(i == CharactersRenderInfos.size() - 1)
            {
                lineEndPos = CharactersRenderInfos[i].RenderPosition.x + CharactersRenderInfos[i].DrawOffset.x +
                        CharactersRenderInfos[i].Size.x + GetHorizontalPadding();
                float alignOffset = 0; 

                switch (HorizontalAlignment)
                {
                    case ssGUI::Enums::TextAlignmentHorizontal::LEFT:
                        break;
                
                    case ssGUI::Enums::TextAlignmentHorizontal::CENTER:
                        alignOffset = (GetSize().x * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                        break;
                    
                    case ssGUI::Enums::TextAlignmentHorizontal::RIGHT:
                        alignOffset = GetSize().x - lineEndPos;
                        break;
                }

                for(int j = lineStartIndex; j <= i; j++)
                    CharactersRenderInfos[j].RenderPosition.x += alignOffset;
            }
        }
        
        //Find out how tall all the texts are and align accordingly
        float alignOffset = 0;
        lineStartPos = 0;
        
        for(int i = 0; i < CharactersRenderInfos.size(); i++)
        {
            if(CharactersRenderInfos[i].RenderPosition.y + CharactersRenderInfos[i].DrawOffset.y < lineStartPos)
                lineStartPos = CharactersRenderInfos[i].RenderPosition.y + CharactersRenderInfos[i].DrawOffset.y;
        }

        lineEndPos = CharactersRenderInfos[CharactersRenderInfos.size() - 1].RenderPosition.y + GetVerticalPadding();
        switch (VerticalAlignment)
        {
            case ssGUI::Enums::TextAlignmentVertical::TOP:
                alignOffset = GetVerticalPadding();
                break;
        
            case ssGUI::Enums::TextAlignmentVertical::CENTER:
                alignOffset = (GetSize().y * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                break;
            
            case ssGUI::Enums::TextAlignmentVertical::BOTTOM:
                alignOffset = GetSize().y - lineEndPos;
                break;
        }

        for(int i = 0; i < CharactersRenderInfos.size(); i++)
            CharactersRenderInfos[i].RenderPosition.y += alignOffset;

        FUNC_DEBUG_EXIT();
    }

    void Text::ApplyTextUnderline()
    {
        FUNC_DEBUG_ENTRY();

        auto drawUnderline = [&](int startIndex, int inclusiveEndIndex, glm::u8vec4 underlineColor, float thickness, float underlineOffset)
        {
            DrawingVerticies.push_back(CharactersRenderInfos[startIndex].RenderPosition + GetGlobalPosition() +
                glm::vec2(0, underlineOffset));

            DrawingVerticies.push_back(CharactersRenderInfos[inclusiveEndIndex].RenderPosition + GetGlobalPosition() + 
                glm::vec2(CharactersRenderInfos[inclusiveEndIndex].Advance, underlineOffset));

            DrawingVerticies.push_back(CharactersRenderInfos[inclusiveEndIndex].RenderPosition + GetGlobalPosition() + 
                glm::vec2(CharactersRenderInfos[inclusiveEndIndex].Advance, underlineOffset + thickness));

            DrawingVerticies.push_back(CharactersRenderInfos[startIndex].RenderPosition + GetGlobalPosition() + 
                glm::vec2(0, underlineOffset + thickness));

            for(int i = 0; i < 4; i++)
            {
                DrawingUVs.push_back(glm::vec2());
                DrawingColours.push_back(underlineColor);
            }

            DrawingCounts.push_back(4);
            DrawingProperties.push_back(ssGUI::DrawingProperty());
        };

        //This handles font size and color change
        auto drawMultiColoredUnderline = [&](int startIndex, int inclusiveEndIndex)
        {
            float maxFontSize = -1;
            int maxFontSizeCharIndex = -1;
            
            //Find max font size
            for (int i = startIndex; i <= inclusiveEndIndex; i++)
            {
                if(CurrentCharacterDetails[i].FontIndex != -1 && CurrentCharacterDetails[i].DefaultFontIndex != -1 && 
                    CurrentCharacterDetails[i].FontSize > maxFontSize)
                {
                    maxFontSize = CurrentCharacterDetails[i].FontSize;
                    maxFontSizeCharIndex = i;
                }
            }

            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            if(CurrentCharacterDetails[maxFontSizeCharIndex].FontIndex != -1)
                fontInterface = GetFont(CurrentCharacterDetails[maxFontSizeCharIndex].FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(CurrentCharacterDetails[maxFontSizeCharIndex].DefaultFontIndex)->GetBackendFontInterface();

            //Draw underlines
            glm::u8vec4 currentUnderlineColor = CurrentCharacterDetails[startIndex].CharacterColor;
            int lastIndex = startIndex;
            for (int i = startIndex; i <= inclusiveEndIndex; i++)
            {
                //Check for color change
                if(CurrentCharacterDetails[i].CharacterColor != currentUnderlineColor)
                {
                    drawUnderline
                    (
                        lastIndex, 
                        i-1, 
                        currentUnderlineColor, 
                        fontInterface->GetUnderlineThickness(maxFontSize), 
                        fontInterface->GetUnderlineOffset(maxFontSize)    
                    );
                    
                    currentUnderlineColor = CurrentCharacterDetails[i].CharacterColor;
                    lastIndex = i;
                }

                //last index, draw underline
                if(i == inclusiveEndIndex)
                {
                    drawUnderline
                    (
                        lastIndex, 
                        i, 
                        currentUnderlineColor, 
                        fontInterface->GetUnderlineThickness(maxFontSize), 
                        fontInterface->GetUnderlineOffset(maxFontSize)    
                    );
                }
            }
        };

        int underlineStartIndex = -1;

        ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
        for (int i = 0; i < CurrentCharacterDetails.size(); i++)
        {
            CharacterDetails curDetail = CurrentCharacterDetails.at(i);
            wchar_t curChar = curDetail.Character;

            //If current character/font is invalid
            if(curChar == L'\0' ||
                (curDetail.FontIndex == -1 && curDetail.DefaultFontIndex == -1) ||
                (curDetail.FontIndex != -1 && GetFont(curDetail.FontIndex) == nullptr) ||
                (curDetail.DefaultFontIndex != -1 && GetDefaultFont(curDetail.DefaultFontIndex) == nullptr))
            {               
                continue;
            }
            
            if(curDetail.FontIndex != -1)
                fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
            
            //If underline started
            if(underlineStartIndex != -1)
            {
                //If current charcter is at newline or
                //If current character doesn't need underline
                if(CharactersRenderInfos[i].CharacterAtNewline || !curDetail.Underlined)
                {
                    //Break underline
                    drawMultiColoredUnderline(underlineStartIndex, i - 1);

                    //Reset status
                    underlineStartIndex = -1;
                }
            }

            //If underline not started
            if(underlineStartIndex == -1)
            {
                //Record underline if needed
                if(curDetail.Underlined)
                    underlineStartIndex = i;
            }

            //Check if last index and underline needed
            if(underlineStartIndex != -1 && i == CurrentCharacterDetails.size() - 1)
            {
                //Draw underline
                drawMultiColoredUnderline(underlineStartIndex, i);
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ConstructRenderInfo()
    {
        FUNC_DEBUG_ENTRY();
        
        glm::vec2 drawPos = GetGlobalPosition();

        //Drawing background
        DrawingVerticies.push_back(drawPos);
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPos + glm::vec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPos + glm::vec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPos + glm::vec2(0, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        if(GetFontsCount() == 0 && GetDefaultFontsCount() == 0)
        {
            DEBUG_LINE("Failed to find any fonts");
            FUNC_DEBUG_EXIT();
            return;
        }

        if(RecalculateTextNeeded)
            ComputeCharactersPositionAndSize(); 

        for (int i = 0; i < CurrentCharacterDetails.size(); i++)
        {
            // Add the glyph to the vertices
            DrawCharacter(drawPos, CharactersRenderInfos[i], CurrentCharacterDetails[i]);
        }
        
        ApplyTextUnderline();

        FUNC_DEBUG_EXIT();
    }

    void Text::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, 
                ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //Check any changes to default fonts
        //TODO: Maybe need optimization
        bool defaultFontsChanged = false; 
        if(LastDefaultFonts.size() != DefaultFonts.size()) 
            defaultFontsChanged = true; 
        else 
        { 
            for(int i = 0; i < DefaultFonts.size(); i++) 
            { 
                if(LastDefaultFonts[i] != DefaultFonts[i]) 
                { 
                    defaultFontsChanged = true; 
                    break; 
                } 
            } 
        } 
 
        if(defaultFontsChanged) 
        { 
            LastDefaultFonts = DefaultFonts; 
            RecalculateTextNeeded = true; 
            RedrawObject(); 
        }

        Widget::MainLogic(inputInterface, globalInputStatus, windowInputStatus, mainWindow);
    }
    
    const std::string Text::ListenerKey = "Text";

    Text::Text() :  CurrentText(), RecalculateTextNeeded(false), OverrideCharactersDetails(), 
                    CharactersRenderInfos(), CurrentCharacterDetails(), Overflow(false), FontSize(15), TextColor(glm::u8vec4(0, 0, 0, 255)), 
                    TextUnderline(false), MultilineAllowed(true), WrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING), 
                    HorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT), VerticalAlignment(ssGUI::Enums::TextAlignmentVertical::TOP), 
                    CurrentFonts(), HorizontalPadding(5), VerticalPadding(5), CharacterSpace(0), LineSpace(0), TabSize(4), LastDefaultFonts()
    {
        SetBackgroundColor(glm::ivec4(255, 255, 255, 0));
        SetBlockInput(false);
        SetInteractable(false);

        auto sizeChangedCallback = ssGUI::Factory::Create<ssGUI::EventCallbacks::SizeChangedEventCallback>();
        sizeChangedCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                static_cast<ssGUI::Text*>(src)->RecalculateTextNeeded = true;
            }
        );

        AddEventCallback(sizeChangedCallback);
    }

    Text::~Text()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }
    
    void Text::ComputeCharactersPositionAndSize()
    {
        FUNC_DEBUG_ENTRY();
        if(GetFontsCount() == 0 && GetDefaultFontsCount() == 0)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        RedrawObject();
        RecalculateTextNeeded = false;
        Overflow = false;

        ConstructCharacterDetails();
        
        FormatNewlinesCharacters();

        AssignSupportedFont();

        CharactersRenderInfos.clear();

        //Empty text guard
        if(CurrentText.empty() && OverrideCharactersDetails.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Populate CharactersRenderInfos list
        for(int i = 0; i < CurrentCharacterDetails.size(); i++)
            CharactersRenderInfos.push_back(ssGUI::CharacterRenderInfo());
        
        if(WrappingMode == ssGUI::Enums::TextWrapping::WORD_WRAPPING)
            ConstructRenderInfosForWordWrapping();
        else if(WrappingMode == ssGUI::Enums::TextWrapping::CHARACTER_WRAPPING)
            ConstructRenderInfosForCharacterWrapping();
        else
            ConstructRenderInfosForNoWrapping();

        ApplyFontLineSpacing();
        ApplyTextAlignment();

        FUNC_DEBUG_EXIT();
    }
    
    void Text::SetText(std::wstring text)
    {
        RecalculateTextNeeded = true;
        CurrentText = text;
        RedrawObject();
    }

    void Text::SetText(std::string text)
    {
        RecalculateTextNeeded = true;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        CurrentText = converter.from_bytes(text);
        RedrawObject();
    }

    std::wstring Text::GetText() const
    {
        return CurrentText;
    }

    int Text::GetCharacterCount() const
    {
        return CurrentText.size();
    }

    ssGUI::CharacterRenderInfo Text::GetCharacterRenderInfo(int index) const
    {
        if(index < 0 || index >= CharactersRenderInfos.size())
            return ssGUI::CharacterRenderInfo();
        
        return CharactersRenderInfos[index];
    }
    
    void Text::SetOverrideCharacterDetails(int index, ssGUI::CharacterDetails details)
    {
        if(index < 0 || index >= OverrideCharactersDetails.size())
            return;

        OverrideCharactersDetails[index] = details;

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::CharacterDetails Text::GetOverrideCharacterDetails(int index) const
    {
        if(index < 0 || index >= OverrideCharactersDetails.size())
            return ssGUI::CharacterDetails();
        
        return OverrideCharactersDetails[index];
    }

    int Text::GetOverrideCharactersDetailsCount() const
    {
        return OverrideCharactersDetails.size();
    }

    void Text::AddOverrideCharacterDetails(int index, ssGUI::CharacterDetails details)
    {
        if(index < 0 || index > OverrideCharactersDetails.size())
            return;

        OverrideCharactersDetails.insert(OverrideCharactersDetails.begin() + index, details);

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddOverrideCharacterDetails(ssGUI::CharacterDetails details)
    {
        OverrideCharactersDetails.push_back(details);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::RemoveOverrideCharacterDetails(int index)
    {
        if(index < 0 || index >= OverrideCharactersDetails.size())
            return;

        OverrideCharactersDetails.erase(OverrideCharactersDetails.begin() + index);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::ClearAllOverrideCharacterDetails()
    {
        OverrideCharactersDetails.clear();
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    glm::vec2 Text::GetCharacterGlobalPosition(int index, bool topLeftCorner)
    {
        if(index < 0 || index >= CharactersRenderInfos.size())
            return glm::vec2();
        
        return GetGlobalPosition() + 
                (topLeftCorner ? 
                CharactersRenderInfos[index].RenderPosition + CharactersRenderInfos[index].DrawOffset :
                CharactersRenderInfos[index].RenderPosition);
    }

    bool Text::IsOverflow() const
    {
        return Overflow;
    }

    void Text::SetFontSize(float size)
    {
        FontSize = size;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetFontSize() const
    {
        return FontSize;
    }

    void Text::SetTextColor(glm::u8vec4 color)
    {
        TextColor = color;
        RecalculateTextNeeded = true;   //Setting text color requires reconstruction of character details
        RedrawObject();
    }

    glm::u8vec4 Text::GetTextColor() const
    {
        return TextColor;
    } 

    void Text::SetTextUnderlined(bool underline)
    {
        TextUnderline = underline;
    }

    bool Text::IsTextUnderlined() const
    {
        return TextUnderline;
    }

    void Text::SetMultilineAllowed(bool multiline)
    {
        MultilineAllowed = multiline;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    bool Text::IsMultilineAllowed() const
    {
        return MultilineAllowed;
    }   

    void Text::SetWrappingMode(ssGUI::Enums::TextWrapping wrappingMode)
    {
        WrappingMode = wrappingMode;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::TextWrapping Text::GetWrappingMode() const
    {
        return WrappingMode;
    }

    void Text::SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal align)
    {
        HorizontalAlignment = align;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::TextAlignmentHorizontal Text::GetHorizontalAlignment() const
    {
        return HorizontalAlignment;
    }

    void Text::SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical align)
    {
        VerticalAlignment = align;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::TextAlignmentVertical Text::GetVerticalAlignment() const
    {
        return VerticalAlignment;
    }

    void Text::AddFont(ssGUI::Font* font)
    {
        if(font == nullptr)
            return;

        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);
        
        CurrentFonts.push_back(font);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddFont(ssGUI::Font* font, int index)
    {
        if(font == nullptr)
            return;
        
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);
        
        CurrentFonts.insert(CurrentFonts.begin() + index, font);
        RecalculateTextNeeded = true;
        RedrawObject();
    }
    
    ssGUI::Font* Text::GetFont(int index) const
    {
        if(index < 0 || index >= CurrentFonts.size())
            return nullptr;
        
        return CurrentFonts[index];
    }

    void Text::SetFont(ssGUI::Font* font, int index)
    {
        if(index < 0 || index >= CurrentFonts.size() || font == nullptr)
            return;

        if(font != CurrentFonts[index] && IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);

        CurrentFonts[index] = font;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::RemoveFont(int index)
    {
        if(index < 0 || index >= CurrentFonts.size())
            return;

        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);

        CurrentFonts.erase(CurrentFonts.begin() + index);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    int Text::GetFontsCount() const
    {
        return CurrentFonts.size();
    }

    void Text::SetHorizontalPadding(float padding)
    {
        HorizontalPadding = padding;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetHorizontalPadding() const
    {
        return HorizontalPadding;
    }

    void Text::SetVerticalPadding(float padding)
    {
        VerticalPadding = padding;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetVerticalPadding() const
    {
        return VerticalPadding;
    }

    void Text::SetCharacterSpace(float charSpace)
    {
        CharacterSpace = charSpace;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetCharacterSpace() const
    {
        return CharacterSpace;
    }

    void Text::SetLineSpace(float lineSpace)
    {
        LineSpace = lineSpace;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetLineSpace() const
    {
        return LineSpace;
    }

    void Text::SetTabSize(float tabSize)
    {
        TabSize = tabSize;
        RecalculateTextNeeded = true;
        RedrawObject();
    }
    
    float Text::GetTabSize() const
    {
        return TabSize;
    }

    int Text::GetContainedCharacterIndexFromPos(glm::vec2 pos)
    {
        if(CharactersRenderInfos.empty())
            return -1;
        
        //Use binary search to find the character
        int startIndex = 0;
        int endIndex = CharactersRenderInfos.size() - 1;
        int midIndex = 0;

        auto checkWithin = [](glm::vec2 checkPos, glm::vec2 charPos, glm::vec2 charSize)->bool
        {
            return checkPos.x > charPos.x && 
                checkPos.x < charPos.x + charSize.x &&
                checkPos.y > charPos.y &&
                checkPos.y < charPos.y + charSize.y;
        };

        //TODO: Add max search iterations
        while (true)
        {
            if(endIndex < startIndex)
                return -1;

            glm::vec2 globalPos = GetGlobalPosition();

            midIndex = (startIndex + endIndex) / 2;
            
            //Check within
            ssGUI::CharacterRenderInfo& midInfo = CharactersRenderInfos[midIndex];
            if(checkWithin(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset, midInfo.Size))
            {
                return midIndex;
            }

            ssGUI::CharacterRenderInfo& startInfo = CharactersRenderInfos[startIndex];
            ssGUI::CharacterRenderInfo& endInfo = CharactersRenderInfos[endIndex];

            //Check if check position is contained
            if(pos.y < globalPos.y + startInfo.LineMinY ||
                pos.y > globalPos.y + endInfo.RenderPosition.y)
            {
                return -1;
            }

            //Check if pos is before midChar or after
            //If above this line
            if(pos.y < globalPos.y + midInfo.LineMinY)
                endIndex = midIndex - 1;

            //If below this line
            else if(pos.y > globalPos.y + midInfo.RenderPosition.y)
                startIndex = midIndex + 1;

            //If within this line
            else
            {
                //Before this character
                if(pos.x < globalPos.x + midInfo.RenderPosition.x + midInfo.DrawOffset.x)
                    endIndex = midIndex - 1;

                //After this character
                else if(pos.x > globalPos.x + midInfo.RenderPosition.x + midInfo.DrawOffset.x)
                    startIndex = midIndex + 1;

                //Not contained
                else
                    return -1;
            }
        }
    }

    int Text::GetNearestCharacterIndexFromPos(glm::vec2 pos)
    {
        if(CharactersRenderInfos.empty())
            return -1;
        
        //Use binary search to find the character
        int startIndex = 0;
        int endIndex = CharactersRenderInfos.size() - 1;
        int midIndex = 0;

        auto checkWithin = [](glm::vec2 checkPos, glm::vec2 charPos, glm::vec2 charSize)->bool
        {
            return checkPos.x > charPos.x && 
                checkPos.x < charPos.x + charSize.x &&
                checkPos.y > charPos.y &&
                checkPos.y < charPos.y + charSize.y;
        };

        //TODO: Add max search iterations
        while (true)
        {
            if(endIndex < startIndex)
                return -1;

            glm::vec2 globalPos = GetGlobalPosition();

            midIndex = (startIndex + endIndex) / 2;

            ssGUI::CharacterRenderInfo& midInfo = CharactersRenderInfos[midIndex];
            ssGUI::CharacterRenderInfo& startInfo = CharactersRenderInfos[startIndex];
            ssGUI::CharacterRenderInfo& endInfo = CharactersRenderInfos[endIndex];

            //Check closest
            if(startIndex == endIndex && endIndex == midIndex)
                return midIndex;

            if(startIndex == midIndex)
            {                
                return glm::distance2(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset + midInfo.Size * 0.5f) < 
                        glm::distance2(pos, globalPos + endInfo.RenderPosition + endInfo.DrawOffset + endInfo.Size * 0.5f) ?
                        midIndex : endIndex;
            }

            if(endIndex == midIndex)
            {
                return glm::distance2(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset + midInfo.Size * 0.5f) < 
                        glm::distance2(pos, globalPos + startInfo.RenderPosition + startInfo.DrawOffset + startInfo.Size * 0.5f) ?
                        midIndex : startIndex;
            }

            //Check if pos is before midChar or after
            if(glm::distance2(pos, globalPos + startInfo.RenderPosition + startInfo.DrawOffset + startInfo.Size * 0.5f) < 
                glm::distance2(pos, globalPos + endInfo.RenderPosition + endInfo.DrawOffset + endInfo.Size * 0.5f))
            {
                endIndex = midIndex;
            }
            else
            {
                startIndex = midIndex;
            }
        }
    }

    void Text::AddDefaultFont(ssGUI::Font* font)
    {
        if(font == nullptr)
            return;
        
        DefaultFonts.push_back(font);
    }

    void Text::AddDefaultFont(ssGUI::Font* font, int index)
    {
        if(font == nullptr)
            return;
        
        DefaultFonts.insert(DefaultFonts.begin() + index, font);
    }
    
    ssGUI::Font* Text::GetDefaultFont(int index)
    {
        if(index < 0 || index >= DefaultFonts.size())
            return nullptr;
        
        return DefaultFonts[index];
    }

    void Text::SetDefaultFont(ssGUI::Font* font, int index)
    {
        if(index < 0 || index >= DefaultFonts.size() || font == nullptr)
            return;

        DefaultFonts[index] = font;
    }

    void Text::RemoveDefaultFont(int index)
    {
        if(index < 0 || index >= DefaultFonts.size())
            return;

        DefaultFonts.erase(DefaultFonts.begin() + index);
    }

    int Text::GetDefaultFontsCount()
    {
        return DefaultFonts.size();
    }

    ssGUI::Enums::GUIObjectType Text::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::TEXT;
    }

    Text* Text::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        Text* temp = new Text(*this);
        CloneExtensionsAndEventCallbacks(temp);
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                FUNC_DEBUG_EXIT();
                return nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
        return temp;
    }
}