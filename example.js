let clickCount = 0;
let isCheckboxChecked = false;
let selectedRadio = 0;
let selectedMenu = 1;
let selectedToggle = 2;
let inputState = "Текст";

const radioOptions = ["Option 1", "Option 2", "Option 3"];
const menuOptions = ["Start game", "Settings", "Exit the game"];
const toggleOptions = ["Easy", "Medium", "Hard"];

function onButtonClick() {
    clickCount++;
}

function onCheckboxChange() {
    isCheckboxChecked = !isCheckboxChecked;
}
function onRadioChange(newIndex) {
    selectedRadio = newIndex;
}

function onMenuChange(newIndex) {
    selectedMenu = newIndex;
}

function onToggleChange(newIndex) {
    selectedToggle = newIndex;
}

function save(newText){
    inputState = newText;
}

function render() {
    return JSON.stringify({
        boxType: "vbox",
        content: [
            {
                type: "text",
                content: "=== Test all framework components ==="
            },
            { type: "separator" },
            {
                type: "text",
                content: "number of clicks : " + clickCount
            },
            {
                type: "button",
                content: "Click on me",
                action: "onButtonClick"
            },
            { type: "separator" },
            {
                type: "checkbox",
                content: "I agree to the license terms",
                checked: isCheckboxChecked,
                action: "onCheckboxChange"
            },
            { type: "separator" },
            {
                type: "text",
                content: "Select your favourite option (Radiobox):"
            },
            {
                type: "radiobox",
                content: radioOptions,
                selected: selectedRadio,
                action: "onRadioChange"
            },
            { type: "separator" },
            {
                type: "text",
                content: "Main menu (Menu):"
            },
            {
                type: "menu",
                content: menuOptions,
                selected: selectedMenu,
                action: "onMenuChange"
            },
            { type: "separator" },
            {
                type: "text",
                content: "Choose the difficulty level (Toggle):"
            },
            {
                type: "toggle",
                content: toggleOptions,
                selected: selectedToggle,
                action: "onToggleChange"
            },
            { type: "separator" },
            {
                type: "input",
                placeholder: "Enter text....",
                content: inputState,
                action: "save"
            },
            {
                type: "text",
                content: inputState ? inputState : "Тексту поки немає"
            }
        ],
        IsBorder: true,
        BorderColor: { r: 252, g: 186, b: 3 },
    });
}