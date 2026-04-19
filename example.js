/**
 * ==========================================
 * Application State
 * Represents the single source of truth for the UI.
 * ==========================================
 */
let userName = "";
let clickCount = 0;
let notificationsEnabled = true;

// Navigation and selection states
let selectedMenuItem = 1;     // 0: Profile, 1: Settings, 2: About
let selectedTheme = 0;        // 0: Dark, 1: Light, 2: System Default
let selectedDifficulty = 1;   // 0: Easy, 1: Normal, 2: Hard

/**
 * ==========================================
 * Action Handlers
 * These functions are triggered by the C++ engine 
 * when the user interacts with the UI components.
 * ==========================================
 */

// Triggered when the user presses Enter in the input field
function onInputEnter(text) {
    userName = text;
}

// Triggered when the button is clicked
function onButtonClick() {
    clickCount++;
}

// Toggles the boolean state for the checkbox
function onCheckboxToggle() {
    notificationsEnabled = !notificationsEnabled;
}

// Updates the currently selected index in the sidebar menu
function onMenuSelect(index) {
    selectedMenuItem = index;
}

// Updates the selected theme from the radiobox
function onThemeChange(index) {
    selectedTheme = index;
}

// Updates the selected difficulty from the toggle component
function onDifficultyChange(index) {
    selectedDifficulty = index;
}

/**
 * ==========================================
 * Render Function
 * Called by the C++ engine to build the UI tree.
 * Returns a JSON string describing the layout and components.
 * ==========================================
 */
function render() {
    // Dynamic greeting logic based on the current state
    const greetingText = userName === "" ? "Guest" : userName;

    return JSON.stringify({
        // Root container: vertical layout
        boxType: "vbox",
        content: [
            /**
             * --- HEADER SECTION ---
             * Horizontal box containing the title and greeting.
             */
            {
                type: "hbox",
                content: [
                    { type: "text", content: " 🚀 My CLI Dashboard" },
                    // In an hbox, the separator renders as a vertical line
                    { type: "separator" }, 
                    { type: "text", content: " Welcome, " + greetingText + "!" }
                ]
            },
            
            // In a vbox, the separator renders as a horizontal line
            { type: "separator" }, 
            
            /**
             * --- MAIN LAYOUT ---
             * Split into two main columns using an hbox.
             */
            {
                type: "hbox",
                content: [
                    /**
                     * LEFT COLUMN: Sidebar Navigation
                     */
                    {
                        type: "vbox",
                        content: [
                            { type: "text", content: "Navigation" },
                            { type: "separator" },
                            {
                                type: "menu",
                                content: ["👤 Profile", "⚙️ Settings", "ℹ️ About"],
                                selected: selectedMenuItem,
                                action: "onMenuSelect"
                            }
                        ]
                    },
                    
                    // Vertical divider between sidebar and main content
                    { type: "separator" }, 
                    
                    /**
                     * RIGHT COLUMN: Settings Content
                     */
                    {
                        type: "vbox",
                        content: [
                            // 1. Text Input Field
                            {
                                type: "hbox",
                                content: [
                                    { type: "text", content: "Enter your name: " },
                                    { 
                                        type: "input", 
                                        placeholder: "Type here and press Enter...", 
                                        action: "onInputEnter" 
                                    }
                                ]
                            },
                            { type: "separator" },

                            // 2. Button with Click Counter
                            {
                                type: "hbox",
                                content: [
                                    { 
                                        type: "button", 
                                        content: "Click Me!", 
                                        action: "onButtonClick" 
                                    },
                                    { type: "text", content: " Total clicks: " + clickCount }
                                ]
                            },
                            { type: "separator" },

                            // 3. Checkbox Component
                            {
                                type: "checkbox",
                                content: "Enable Push Notifications",
                                checked: notificationsEnabled,
                                action: "onCheckboxToggle"
                            },
                            { type: "separator" },

                            // 4. Radiobox for Theme Selection
                            { type: "text", content: "App Theme:" },
                            {
                                type: "radiobox",
                                content: ["Dark Mode", "Light Mode", "System Default"],
                                selected: selectedTheme,
                                action: "onThemeChange"
                            },
                            { type: "separator" },

                            // 5. Toggle for Difficulty
                            { type: "text", content: "Difficulty Level:" },
                            {
                                type: "toggle",
                                content: ["Easy", "Normal", "Hard"],
                                selected: selectedDifficulty,
                                action: "onDifficultyChange"
                            }
                        ]
                    }
                ]
            }
        ],
        IsBorder: true, // Draws a border around the entire application window
        BorderColor: { r: 66, g: 135, b: 245 } // RGB color for the border
    });
}