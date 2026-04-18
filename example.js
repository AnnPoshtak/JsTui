function render() {
    const toRender = {
        boxType: "vbox",
        content: [
            {
                type: "text",
                content: "Hello, World!"
            },
            {
                type: "separator",
                content: ""
            },
            {
                type: "text",
                content: "And this is verryyyyyy loooooooooong teeeeeeeeexxxxxxxtttttttt"
            }
        ],
        IsBorder: true,
        BorderColor: {r: 135, g: 52, b: 155},
    };

    return JSON.stringify(toRender);
}

render();