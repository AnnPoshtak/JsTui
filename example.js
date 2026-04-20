/**
 * ==========================================
 * Application State
 * ==========================================
 */
let userName = "";
let volume = 50;

/**
 * ==========================================
 * Action Handlers
 * ==========================================
 */
function onInputEnter(text) {
    userName = text;
}

function onVolumeChange(value) {
    volume = value;
}

/**
 * ==========================================
 * Render Function
 * ==========================================
 */
function render() {
    const greetingText = userName === "" ? "Guest" : userName;

    return JSON.stringify({
        // Головний контейнер тепер - hbox (розділяє екран на вертикальні колонки)
        type: "hbox", 
        content: [
            // --- ЛІВА КОЛОНКА (Вікно профілю) ---
            {
                type: "window",
                title: " 👤 Profile ",
                content: [
                    { type: "text", content: "Welcome back, " + greetingText + "!" },
                    { type: "separator" },
                    { type: "text", content: "Enter your name:" },
                    { 
                        type: "input", 
                        placeholder: "Type here and press Enter...", 
                        action: "onInputEnter" 
                    }
                ]
            },
            
            // --- ПРАВА КОЛОНКА (Вікно налаштувань) ---
            {
                type: "window",
                title: " 🛠️ Audio Settings ",
                content: [
                    { type: "text", content: "Audio Volume: " + volume + "%" },
                    { type: "separator" },
                    {
                        type: "slider",
                        content: "🔊 ",
                        value: volume,
                        action: "onVolumeChange"
                    }
                ]
            }
        ]
    });
}