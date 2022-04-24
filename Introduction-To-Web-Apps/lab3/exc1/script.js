

const show_prompt = () => {
    document.getElementById("prompt").style = "display: block";
}

const get_name = () => {
    document.getElementById("feedback").innerHTML = document.getElementById("name-prompt").value;
}