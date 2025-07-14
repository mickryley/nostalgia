#include "imgui_presets.h"

#include "imgui.h"
#include <filesystem>

namespace nostalgia::gui::style {

    void style_shapes(){
        ImGuiStyle& style = ImGui::GetStyle();

        // Transparency
        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.5f; // soften disabled

        // Window settings
        style.WindowPadding = ImVec2(14.0f, 14.0f);  
        style.WindowRounding = 6.0f;                 
        style.WindowBorderSize = 1.0f;               

        // Child windows
        style.ChildRounding = 6.0f;
        style.ChildBorderSize = 1.0f;

        // Popups and tooltips
        style.PopupRounding = 6.0f;
        style.PopupBorderSize = 1.0f;

        // Frame (Buttons, Checkboxes, Sliders)
        style.FramePadding = ImVec2(8.0f, 6.0f);
        style.FrameRounding = 4.0f;
        style.FrameBorderSize = 1.0f;

        // Spacing
        style.ItemSpacing = ImVec2(10.0f, 8.0f);
        style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);

        // Scrollbar
        style.ScrollbarSize = 14.0f;
        style.ScrollbarRounding = 7.0f;

        // Sliders/Grabs
        style.GrabMinSize = 8.0f;
        style.GrabRounding = 4.0f;

        // Tabs
        style.TabRounding = 4.0f;
        style.TabBorderSize = 0.0f;  
        style.TabBarBorderSize = 0.0f;
        style.TabBarOverlineSize = 2.0f;    // Thin highlight line on active tab

        // Misc
        style.SeparatorTextBorderSize = 1.0f;
        style.SeparatorTextPadding = ImVec2(6.0f, 2.0f);
        style.SeparatorTextAlign = ImVec2(0.5f, 0.5f);

        // Alignment
        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);    // Left
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);     // Centered
        style.SelectableTextAlign = ImVec2(0.0f, 0.5f); // Left

        // Touch and Display Padding
        style.DisplayWindowPadding = ImVec2(20.0f, 20.0f);
        style.DisplaySafeAreaPadding = ImVec2(4.0f, 4.0f);

        // Anti-Aliasing
        style.AntiAliasedLines = true;
        style.AntiAliasedLinesUseTex = true;
        style.AntiAliasedFill = true;

        // Geometry Detail
        style.CurveTessellationTol = 1.25f;
        style.CircleTessellationMaxError = 0.25f;
    }

    void load_local_fonts(){
        if (std::filesystem::exists("assets/fonts/CascadiaCodeNF.ttf")){
            ImGuiIO& io = ImGui::GetIO();
            ImFont* loadedFont = io.Fonts->AddFontFromFileTTF("assets/fonts/CascadiaCodeNF.ttf",
                 12.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
            if (loadedFont != nullptr)
                io.FontDefault = loadedFont;
        }
    }

    void style_colors_taupe(){
        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        // == Backgrounds ==
        colors[ImGuiCol_WindowBg]            = ImVec4(0.11f, 0.09f, 0.07f, 1.00f); // Rich brown (vintage paper)
        colors[ImGuiCol_ChildBg]             = ImVec4(0.13f, 0.11f, 0.08f, 1.00f); // Slightly lighter
        colors[ImGuiCol_PopupBg]             = ImVec4(0.16f, 0.13f, 0.09f, 0.97f); // Warm sepia
        colors[ImGuiCol_ModalWindowDimBg]    = ImVec4(0.08f, 0.07f, 0.04f, 0.65f);
        colors[ImGuiCol_NavWindowingDimBg]   = ImVec4(0.12f, 0.10f, 0.08f, 0.20f);

        // == Text ==
        colors[ImGuiCol_Text]                = ImVec4(0.94f, 0.89f, 0.82f, 1.00f); // Warm off-white
        colors[ImGuiCol_TextDisabled]        = ImVec4(0.66f, 0.60f, 0.51f, 1.00f); // Faded tan
        colors[ImGuiCol_TextSelectedBg]      = ImVec4(1.00f, 0.89f, 0.30f, 0.38f); // Warm yellow
        colors[ImGuiCol_DragDropTarget]      = ImVec4(1.00f, 0.85f, 0.35f, 0.90f);

        // == Borders ==
        colors[ImGuiCol_Border]              = ImVec4(0.49f, 0.38f, 0.22f, 0.38f); // Light brown
        colors[ImGuiCol_BorderShadow]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // == Headers / Collapsing Headers ==
        colors[ImGuiCol_Header]              = ImVec4(0.93f, 0.80f, 0.25f, 0.65f); // Yellow highlight
        colors[ImGuiCol_HeaderHovered]       = ImVec4(0.99f, 0.90f, 0.35f, 0.85f);
        colors[ImGuiCol_HeaderActive]        = ImVec4(1.00f, 0.95f, 0.49f, 1.00f);

        // == Title Bar ==
        colors[ImGuiCol_TitleBg]             = ImVec4(0.19f, 0.15f, 0.09f, 1.00f); // Slightly lighter brown
        colors[ImGuiCol_TitleBgActive]       = ImVec4(0.23f, 0.19f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]    = ImVec4(0.13f, 0.11f, 0.07f, 0.80f);

        // == Frames (Input, Slider, Checkbox) ==
        colors[ImGuiCol_FrameBg]             = ImVec4(0.24f, 0.18f, 0.12f, 0.54f); // Almond/tan
        colors[ImGuiCol_FrameBgHovered]      = ImVec4(0.36f, 0.23f, 0.13f, 0.75f); // Coffee
        colors[ImGuiCol_FrameBgActive]       = ImVec4(0.40f, 0.28f, 0.13f, 0.95f);

        // == Buttons ==
        colors[ImGuiCol_Button]              = ImVec4(0.33f, 0.23f, 0.18f, 0.66f); // Muted brown/clay
        colors[ImGuiCol_ButtonHovered]       = ImVec4(0.47f, 0.33f, 0.20f, 0.88f); // Slightly lighter
        colors[ImGuiCol_ButtonActive]        = ImVec4(0.70f, 0.55f, 0.20f, 1.00f); // Warm gold/yellow

        // == Tabs ==
        colors[ImGuiCol_Tab]                 = ImVec4(0.21f, 0.15f, 0.10f, 0.80f); // Soft brown
        colors[ImGuiCol_TabHovered]          = ImVec4(1.00f, 0.85f, 0.22f, 0.97f); // Yellow highlight
        colors[ImGuiCol_TabActive]           = ImVec4(0.96f, 0.82f, 0.32f, 1.00f); // Light yellow
        colors[ImGuiCol_TabUnfocused]        = ImVec4(0.13f, 0.11f, 0.08f, 0.58f);
        colors[ImGuiCol_TabUnfocusedActive]  = ImVec4(0.29f, 0.21f, 0.14f, 1.00f);

        // == Scrollbars ==
        colors[ImGuiCol_ScrollbarBg]         = ImVec4(0.13f, 0.11f, 0.07f, 0.67f);
        colors[ImGuiCol_ScrollbarGrab]       = ImVec4(0.40f, 0.28f, 0.13f, 0.38f);
        colors[ImGuiCol_ScrollbarGrabHovered]= ImVec4(0.55f, 0.40f, 0.18f, 0.74f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.93f, 0.80f, 0.25f, 1.00f);

        // == Sliders ==
        colors[ImGuiCol_SliderGrab]          = ImVec4(0.99f, 0.91f, 0.33f, 0.70f); // yellow
        colors[ImGuiCol_SliderGrabActive]    = ImVec4(1.00f, 0.95f, 0.40f, 1.00f);

        // == Checkmarks ==
        colors[ImGuiCol_CheckMark]           = ImVec4(1.00f, 0.85f, 0.25f, 1.00f);

        // == Resize Grips ==
        colors[ImGuiCol_ResizeGrip]          = ImVec4(1.00f, 0.85f, 0.25f, 0.38f);
        colors[ImGuiCol_ResizeGripHovered]   = ImVec4(1.00f, 0.93f, 0.30f, 0.77f);
        colors[ImGuiCol_ResizeGripActive]    = ImVec4(1.00f, 0.95f, 0.45f, 1.00f);

        // == Menu Bar ==
        colors[ImGuiCol_MenuBarBg]           = ImVec4(0.17f, 0.14f, 0.10f, 1.00f);

        // == Separators ==
        colors[ImGuiCol_Separator]           = ImVec4(0.95f, 0.80f, 0.23f, 0.38f);
        colors[ImGuiCol_SeparatorHovered]    = ImVec4(1.00f, 0.93f, 0.30f, 0.80f);
        colors[ImGuiCol_SeparatorActive]     = ImVec4(1.00f, 0.95f, 0.45f, 1.00f);

        // == Navigation ==
        colors[ImGuiCol_NavHighlight]        = ImVec4(1.00f, 0.85f, 0.22f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]=ImVec4(1.00f, 0.93f, 0.30f, 0.70f);

        // == Plots ==
        colors[ImGuiCol_PlotLines]           = ImVec4(0.83f, 0.75f, 0.65f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]    = ImVec4(1.00f, 0.89f, 0.26f, 1.00f);
        colors[ImGuiCol_PlotHistogram]       = ImVec4(0.96f, 0.87f, 0.66f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]= ImVec4(1.00f, 0.93f, 0.30f, 1.00f);

        // == Docking ==
        colors[ImGuiCol_DockingPreview]      = ImVec4(1.00f, 0.88f, 0.28f, 0.46f);
        colors[ImGuiCol_DockingEmptyBg]      = ImVec4(0.12f, 0.10f, 0.08f, 1.00f);

        // == Tree Lines ==
        colors[ImGuiCol_TreeLines]           = ImVec4(0.48f, 0.38f, 0.20f, 0.43f);
    }

    void style_colors_slate(){
        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        // == Backgrounds ==
        colors[ImGuiCol_WindowBg]            = ImVec4(0.06f, 0.07f, 0.11f, 1.00f); // Near-black navy (midnight blue)
        colors[ImGuiCol_ChildBg]             = ImVec4(0.08f, 0.09f, 0.13f, 1.00f); // Slightly lighter
        colors[ImGuiCol_PopupBg]             = ImVec4(0.12f, 0.11f, 0.17f, 0.97f); // Wet concrete
        colors[ImGuiCol_ModalWindowDimBg]    = ImVec4(0.06f, 0.07f, 0.11f, 0.68f); // Night
        colors[ImGuiCol_NavWindowingDimBg]   = ImVec4(0.09f, 0.09f, 0.13f, 0.25f);

        // == Text ==
        colors[ImGuiCol_Text]                = ImVec4(0.93f, 0.93f, 0.93f, 1.00f); // Slightly blue-tinted white
        colors[ImGuiCol_TextDisabled]        = ImVec4(0.47f, 0.49f, 0.55f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]      = ImVec4(1.00f, 0.87f, 0.22f, 0.35f); // Yellow light
        colors[ImGuiCol_DragDropTarget]      = ImVec4(1.00f, 0.87f, 0.22f, 0.85f);

        // == Borders ==
        colors[ImGuiCol_Border]              = ImVec4(0.22f, 0.23f, 0.32f, 0.55f); // Steel edge
        colors[ImGuiCol_BorderShadow]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // == Headers / Collapsing Headers ==
        colors[ImGuiCol_Header]              = ImVec4(1.00f, 0.85f, 0.15f, 0.72f); // Neon gold
        colors[ImGuiCol_HeaderHovered]       = ImVec4(1.00f, 0.93f, 0.33f, 0.93f);
        colors[ImGuiCol_HeaderActive]        = ImVec4(1.00f, 0.97f, 0.55f, 1.00f);

        // == Title Bar ==
        colors[ImGuiCol_TitleBg]             = ImVec4(0.10f, 0.11f, 0.19f, 1.00f); // Dark navy
        colors[ImGuiCol_TitleBgActive]       = ImVec4(0.13f, 0.15f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]    = ImVec4(0.09f, 0.09f, 0.15f, 0.90f);

        // == Frames (Input, Slider, Checkbox) ==
        colors[ImGuiCol_FrameBg]             = ImVec4(0.15f, 0.17f, 0.21f, 0.55f); // Dusk
        colors[ImGuiCol_FrameBgHovered]      = ImVec4(0.22f, 0.25f, 0.33f, 0.82f);
        colors[ImGuiCol_FrameBgActive]       = ImVec4(0.34f, 0.33f, 0.46f, 0.99f);

        // == Buttons ==
        colors[ImGuiCol_Button]              = ImVec4(0.22f, 0.23f, 0.32f, 0.62f); // Asphalt blue-grey
        colors[ImGuiCol_ButtonHovered]       = ImVec4(1.00f, 0.85f, 0.15f, 0.80f); // Gold pop
        colors[ImGuiCol_ButtonActive]        = ImVec4(1.00f, 0.93f, 0.33f, 1.00f);

        // == Tabs ==
        colors[ImGuiCol_Tab]                 = ImVec4(0.12f, 0.14f, 0.18f, 0.85f);
        colors[ImGuiCol_TabHovered]          = ImVec4(1.00f, 0.85f, 0.15f, 0.96f);
        colors[ImGuiCol_TabActive]           = ImVec4(1.00f, 0.93f, 0.33f, 1.00f);
        colors[ImGuiCol_TabUnfocused]        = ImVec4(0.10f, 0.11f, 0.17f, 0.55f);
        colors[ImGuiCol_TabUnfocusedActive]  = ImVec4(0.19f, 0.21f, 0.29f, 1.00f);

        // == Scrollbars ==
        colors[ImGuiCol_ScrollbarBg]         = ImVec4(0.09f, 0.09f, 0.14f, 0.69f);
        colors[ImGuiCol_ScrollbarGrab]       = ImVec4(0.22f, 0.23f, 0.32f, 0.50f);
        colors[ImGuiCol_ScrollbarGrabHovered]= ImVec4(1.00f, 0.85f, 0.15f, 0.75f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.93f, 0.33f, 1.00f);

        // == Sliders ==
        colors[ImGuiCol_SliderGrab]          = ImVec4(1.00f, 0.87f, 0.22f, 0.69f);
        colors[ImGuiCol_SliderGrabActive]    = ImVec4(1.00f, 0.93f, 0.33f, 1.00f);

        // == Checkmarks ==
        colors[ImGuiCol_CheckMark]           = ImVec4(1.00f, 0.87f, 0.22f, 1.00f);

        // == Resize Grips ==
        colors[ImGuiCol_ResizeGrip]          = ImVec4(1.00f, 0.85f, 0.15f, 0.43f);
        colors[ImGuiCol_ResizeGripHovered]   = ImVec4(1.00f, 0.93f, 0.33f, 0.79f);
        colors[ImGuiCol_ResizeGripActive]    = ImVec4(1.00f, 0.97f, 0.55f, 1.00f);

        // == Menu Bar ==
        colors[ImGuiCol_MenuBarBg]           = ImVec4(0.08f, 0.09f, 0.14f, 1.00f);

        // == Separators ==
        colors[ImGuiCol_Separator]           = ImVec4(1.00f, 0.85f, 0.15f, 0.45f);
        colors[ImGuiCol_SeparatorHovered]    = ImVec4(1.00f, 0.93f, 0.33f, 0.84f);
        colors[ImGuiCol_SeparatorActive]     = ImVec4(1.00f, 0.97f, 0.55f, 1.00f);

        // == Navigation ==
        colors[ImGuiCol_NavHighlight]        = ImVec4(1.00f, 0.85f, 0.15f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]=ImVec4(1.00f, 0.93f, 0.33f, 0.73f);

        // == Plots ==
        colors[ImGuiCol_PlotLines]           = ImVec4(0.64f, 0.62f, 0.78f, 1.00f); 
        colors[ImGuiCol_PlotLinesHovered]    = ImVec4(1.00f, 0.87f, 0.22f, 1.00f);
        colors[ImGuiCol_PlotHistogram]       = ImVec4(1.00f, 0.93f, 0.33f, 0.75f);
        colors[ImGuiCol_PlotHistogramHovered]= ImVec4(1.00f, 0.85f, 0.15f, 1.00f);

        // == Docking ==
        colors[ImGuiCol_DockingPreview]      = ImVec4(1.00f, 0.85f, 0.15f, 0.45f);
        colors[ImGuiCol_DockingEmptyBg]      = ImVec4(0.06f, 0.07f, 0.11f, 1.00f);

        // == Tree Lines ==
        colors[ImGuiCol_TreeLines]           = ImVec4(0.25f, 0.24f, 0.35f, 0.35f);
    }

    void style_colors_charcoal(){
        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        // == Backgrounds ==
        colors[ImGuiCol_WindowBg]            = ImVec4(0.07f, 0.08f, 0.09f, 1.00f);
        colors[ImGuiCol_ChildBg]             = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_PopupBg]             = ImVec4(0.13f, 0.13f, 0.15f, 0.97f);
        colors[ImGuiCol_ModalWindowDimBg]    = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]   = ImVec4(0.03f, 0.03f, 0.03f, 0.15f);

        // == Text ==
        colors[ImGuiCol_Text]                = ImVec4(0.93f, 0.92f, 0.87f, 1.00f); 
        colors[ImGuiCol_TextDisabled]        = ImVec4(0.45f, 0.44f, 0.40f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]      = ImVec4(1.00f, 0.84f, 0.17f, 0.34f);
        colors[ImGuiCol_DragDropTarget]      = ImVec4(1.00f, 0.84f, 0.17f, 0.85f);

        // == Borders ==
        colors[ImGuiCol_Border]              = ImVec4(0.16f, 0.17f, 0.17f, 0.52f);
        colors[ImGuiCol_BorderShadow]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // == Headers / Collapsing Headers ==
        colors[ImGuiCol_Header]              = ImVec4(1.00f, 0.84f, 0.17f, 0.63f);
        colors[ImGuiCol_HeaderHovered]       = ImVec4(1.00f, 0.93f, 0.41f, 0.90f);
        colors[ImGuiCol_HeaderActive]        = ImVec4(1.00f, 1.00f, 0.60f, 1.00f);

        // == Title Bar ==
        colors[ImGuiCol_TitleBg]             = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
        colors[ImGuiCol_TitleBgActive]       = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]    = ImVec4(0.10f, 0.11f, 0.13f, 0.86f);

        // == Frames (Input, Slider, Checkbox) ==
        colors[ImGuiCol_FrameBg]             = ImVec4(0.16f, 0.17f, 0.19f, 0.67f);
        colors[ImGuiCol_FrameBgHovered]      = ImVec4(0.22f, 0.22f, 0.23f, 0.85f);
        colors[ImGuiCol_FrameBgActive]       = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);

        // == Buttons ==
        colors[ImGuiCol_Button]              = ImVec4(0.13f, 0.13f, 0.15f, 0.76f);
        colors[ImGuiCol_ButtonHovered]       = ImVec4(1.00f, 0.84f, 0.17f, 0.76f);
        colors[ImGuiCol_ButtonActive]        = ImVec4(1.00f, 0.93f, 0.41f, 1.00f);

        // == Tabs ==
        colors[ImGuiCol_Tab]                 = ImVec4(0.10f, 0.11f, 0.13f, 0.92f);
        colors[ImGuiCol_TabHovered]          = ImVec4(1.00f, 0.84f, 0.17f, 0.92f);
        colors[ImGuiCol_TabActive]           = ImVec4(1.00f, 0.93f, 0.41f, 1.00f);
        colors[ImGuiCol_TabUnfocused]        = ImVec4(0.09f, 0.10f, 0.12f, 0.61f);
        colors[ImGuiCol_TabUnfocusedActive]  = ImVec4(0.13f, 0.13f, 0.15f, 1.00f);

        // == Scrollbars ==
        colors[ImGuiCol_ScrollbarBg]         = ImVec4(0.09f, 0.10f, 0.12f, 0.55f);
        colors[ImGuiCol_ScrollbarGrab]       = ImVec4(0.14f, 0.14f, 0.16f, 0.61f);
        colors[ImGuiCol_ScrollbarGrabHovered]= ImVec4(1.00f, 0.84f, 0.17f, 0.81f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.93f, 0.41f, 1.00f);

        // == Sliders ==
        colors[ImGuiCol_SliderGrab]          = ImVec4(1.00f, 0.84f, 0.17f, 0.70f);
        colors[ImGuiCol_SliderGrabActive]    = ImVec4(1.00f, 0.93f, 0.41f, 1.00f);

        // == Checkmarks ==
        colors[ImGuiCol_CheckMark]           = ImVec4(1.00f, 0.84f, 0.17f, 1.00f);

        // == Resize Grips ==
        colors[ImGuiCol_ResizeGrip]          = ImVec4(1.00f, 0.84f, 0.17f, 0.43f);
        colors[ImGuiCol_ResizeGripHovered]   = ImVec4(1.00f, 0.93f, 0.41f, 0.85f);
        colors[ImGuiCol_ResizeGripActive]    = ImVec4(1.00f, 1.00f, 0.60f, 1.00f);

        // == Menu Bar ==
        colors[ImGuiCol_MenuBarBg]           = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);

        // == Separators ==
        colors[ImGuiCol_Separator]           = ImVec4(1.00f, 0.84f, 0.17f, 0.39f);
        colors[ImGuiCol_SeparatorHovered]    = ImVec4(1.00f, 0.93f, 0.41f, 0.81f);
        colors[ImGuiCol_SeparatorActive]     = ImVec4(1.00f, 1.00f, 0.60f, 1.00f);

        // == Navigation ==
        colors[ImGuiCol_NavHighlight]        = ImVec4(1.00f, 0.84f, 0.17f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]=ImVec4(1.00f, 0.93f, 0.41f, 0.73f);

        // == Plots ==
        colors[ImGuiCol_PlotLines]           = ImVec4(0.59f, 0.59f, 0.53f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]    = ImVec4(1.00f, 0.84f, 0.17f, 1.00f);
        colors[ImGuiCol_PlotHistogram]       = ImVec4(1.00f, 0.93f, 0.41f, 0.73f);
        colors[ImGuiCol_PlotHistogramHovered]= ImVec4(1.00f, 0.84f, 0.17f, 1.00f);

        // == Docking ==
        colors[ImGuiCol_DockingPreview]      = ImVec4(1.00f, 0.84f, 0.17f, 0.49f);
        colors[ImGuiCol_DockingEmptyBg]      = ImVec4(0.07f, 0.08f, 0.09f, 1.00f);

        // == Tree Lines ==
        colors[ImGuiCol_TreeLines]           = ImVec4(0.25f, 0.25f, 0.24f, 0.39f);
    }
}