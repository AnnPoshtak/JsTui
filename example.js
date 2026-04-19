let clickCount = 0;
let isCheckboxChecked = false;
let selectedRadio = 0;
let selectedMenu = 1;
let selectedToggle = 2;

const radioOptions = ["Варіант 1", "Варіант 2", "Варіант 3"];
const menuOptions = ["Почати гру", "Налаштування", "Вийти з гри"];
const toggleOptions = ["Легко", "Середньо", "Складно"];

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

function render() {
    return JSON.stringify({
        boxType: "vbox",
        content: [
            {
                type: "text",
                content: "=== Тест усіх компонентів фреймворку ==="
            },
            { type: "separator" },
            {
                type: "text",
                content: "Кількість натискань: " + clickCount
            },
            {
                type: "button",
                content: "Натисни мене!",
                action: "onButtonClick"
            },
            { type: "separator" },
            {
                type: "checkbox",
                content: "Погоджуюсь з умовами ліцензії",
                checked: isCheckboxChecked,
                action: "onCheckboxChange"
            },
            { type: "separator" },
            {
                type: "text",
                content: "Оберіть улюблений варіант (Radiobox):"
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
                content: "Головне меню (Menu):"
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
                content: "Оберіть рівень складності (Toggle):"
            },
            {
                type: "toggle",
                content: toggleOptions,
                selected: selectedToggle,
                action: "onToggleChange"
            }
        ],
        IsBorder: true,
        BorderColor: { r: 252, g: 186, b: 3 }
    });
}