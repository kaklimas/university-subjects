let ul_list = document.getElementById('tasks');

const add = () => {
    const li_node = document.createElement("li");
    const node_text = document.createTextNode("Something to do...");
    li_node.appendChild(node_text);
    ul_list.appendChild(li_node);

    if (ul_list.children.length > 0){
        document.getElementById('remove-button').disabled = false;
        document.getElementById('remove-button').style = "cursor: pointer";
    }
}

const remove = () => {
    if (ul_list.children.length > 0){
        ul_list.removeChild(ul_list.firstElementChild);
    }
    if (ul_list.children.length == 0){
        document.getElementById('remove-button').disabled = true;
        document.getElementById('remove-button').style = "cursor: default";
    }
}