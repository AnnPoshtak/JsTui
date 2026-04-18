function render() {
    const toRender = {
        boxType: "vbox",
        content: [
            {
                type: "text",
                content: "Hello, World!"
            }
        ],
        IsBorder: true,
        BorderColor: "red",
        borderType: ["bold", "double"],
    };

    return JSON.stringify(toRender);
}

render();