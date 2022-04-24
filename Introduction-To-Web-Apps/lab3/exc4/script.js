let control_button = document.getElementById('control-button');
let counter_button = document.getElementById('counter-button');
let counter_field = document.getElementById('counter');

const increase_value = () => {
    counter_field.innerHTML = parseInt(counter_field.innerHTML) + 1;
}

control_button.addEventListener('click', () => {
    if (control_button.innerHTML == "Add event"){
        control_button.innerHTML = "Remove event";
        counter_button.disabled = false;
        counter_button.addEventListener('click', increase_value);
    
    } else {
        control_button.innerHTML = "Add event";
        counter_button.disabled = true;
        counter_field.innerHTML = 0;
        counter_button.removeEventListener('click', increase_value);
    }
})
