#ifndef WORKSPACECONSTANTS_H
#define WORKSPACECONSTANTS_H

// Inline konstanter, til forskellige funktioner
namespace WorkspaceConstants
{
    inline constexpr int firstLayer{0};

    // Konstanter for brugerens valgte arbejdsområde
    inline constexpr int minimumGridWidth{1};
    inline constexpr int maximumGridWidth{20};
    inline constexpr int minimumGridHeight{1};
    inline constexpr int maximumGridHeight{20};
    inline constexpr int defaultGridWidth{4};
    inline constexpr int defaultGridHeight{5};

    // Konstanter for størelse af felter på UI
    inline constexpr int minimumGridCellSize{24};
    inline constexpr int maximumGridCellSize{70};
}

#endif // WORKSPACECONSTANTS_H
