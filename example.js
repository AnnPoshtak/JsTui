/**
 * Application State
 */
let currentTab = 0;
let userName = "";
let volume = 50;
let isAgreed = false;
let clickCount = 0;
let themeIdx = 0;
let lastAction = "Ready";

/**
 * Action Handlers
 */
function onTabChange(index) {
    currentTab = index;
    lastAction = `Switched to tab ${index}`;
}

function onInputEnter(text) {
    userName = text;
    lastAction = `User name set to: ${text}`;
}

function onVolumeChange(value) {
    volume = value;
}

function onToggleCheck() {
    isAgreed = !isAgreed;
    lastAction = `Checkbox is now ${isAgreed}`;
}

function onBtnClick() {
    clickCount++;
    lastAction = `Button clicked ${clickCount} times`;
}

function onThemeChange(index) {
    themeIdx = index;
    lastAction = `Theme changed to index ${index}`;
}

/**
 * Main Render Function
 */
function render() {
    // Dynamic color palette based on theme selection
    const colors = [
        { r: 0, g: 255, b: 255 }, // Cyan
        { r: 255, g: 0, b: 255 }, // Magenta
        { r: 255, g: 255, b: 0 }  // Yellow
    ];
    const activeColor = colors[themeIdx];

    // Prepare content for the active tab
    let tabContent;

    if (currentTab === 0) {
        // TAB: INPUTS & CONTROLS
        tabContent = {
            type: "vbox",
            styles: { flex: true },
            content: [
                { type: "text", content: "Personal Information", styles: { bold: true, color: activeColor } },
                { type: "input", placeholder: "Enter your name...", action: "onInputEnter" },
                { type: "separator" },
                { type: "checkbox", content: "Enable experimental features", checked: isAgreed, action: "onToggleCheck" },
                { type: "separator" },
                { type: "text", content: `System Volume: ${volume}%`, styles: { dim: true } },
                { type: "slider", content: "VOL: ", value: volume, action: "onVolumeChange", styles: { color: activeColor } }
            ]
        };
    } else if (currentTab === 1) {
        // TAB: STYLES SHOWCASE
        tabContent = {
            type: "vbox",
            styles: { flex: true },
            content: [
                { type: "text", content: "Border & Text Styles", styles: { underlined: true, center: true } },
                { 
                    type: "hbox", 
                    styles: { center: true },
                    content: [
                        { type: "text", content: " BOLD ", styles: { bold: true, border: "normal" } },
                        { type: "text", content: " DIM ", styles: { dim: true, border: "rounded" } },
                        { type: "text", content: " BG ", styles: { bgcolor: { r: 50, g: 50, b: 150 }, border: "double" } }
                    ]
                },
                { type: "separator" },
                { type: "text", content: "Choose Global Highlight:", styles: { dim: true } },
                { 
                    type: "radiobox", 
                    content: ["Cyan Neon", "Magenta Punk", "Classic Yellow"], 
                    selected: themeIdx, 
                    action: "onThemeChange" 
                }
            ]
        };
    } else {
        // TAB: INTERACTIVE
        tabContent = {
            type: "vbox",
            styles: { flex: true, center: true },
            content: [
                { type: "text", content: `Counter: ${clickCount}`, styles: { bold: true, color: activeColor } },
                { 
                    type: "button", 
                    content: " [ CLICK TO INCREMENT ] ", 
                    action: "onBtnClick", 
                    styles: { border: "rounded", bold: true } 
                }
            ]
        };
    }

    // Assemble the Final UI Root
    return JSON.stringify({
        type: "vbox",
        content: [
            // Header Bar
            {
                type: "text",
                content: " 🛠️ FRAMEWORK KITCHEN SINK 🛠️ ",
                styles: { 
                    center: true, 
                    bold: true, 
                    bgcolor: activeColor, 
                    color: { r: 0, g: 0, b: 0 } 
                }
            },
            // Main Layout (Sidebar + Content)
            {
                type: "hbox",
                styles: { flex: true },
                content: [
                    {
                        type: "window",
                        title: " Navigation ",
                        styles: { border: "rounded" },
                        content: [
                            {
                                type: "toggle",
                                content: ["Forms", "Styles", "Action"],
                                selected: currentTab,
                                action: "onTabChange"
                            },
                            { type: "separator" },
                            { type: "text", content: `Hello, ${userName || "Guest"}`, styles: { italic: true, color: activeColor } }
                        ]
                    },
                    {
                        type: "window",
                        title: " Workspace ",
                        styles: { border: "double", flex: true },
                        content: [tabContent]
                    }
                ]
            },
            // Footer / Status Bar
            {
                type: "hbox",
                styles: { bgcolor: { r: 30, g: 30, b: 30 } },
                content: [
                    { type: "text", content: ` Status: ${lastAction} `, styles: { dim: true, flex: true } },
                    { type: "text", content: " FTXUI v1.0 ", styles: { color: activeColor } }
                ]
            }
        ]
    });
}