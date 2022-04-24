let blue = document.getElementsByClassName('blue')[0];
let red = document.getElementsByClassName('red')[0];
let yellow = document.getElementsByClassName('yellow')[0];

let feedback = document.getElementById('feedback');
// BUTTONS
let control_button = document.getElementById('control-button');
let restart_button = document.getElementById('restart-button');

// COUNTER
let counter_value = document.getElementById('counter');

let propagation = true;


// EVENT HANDLERS
const blue_prop = (e) => {
    counter_value.innerHTML = parseInt(counter_value.innerHTML) + 1;
    feedback.innerHTML = "Kliknales na niebieski 1";
}
const blue_nprop = (e) => {
    counter_value.innerHTML = parseInt(counter_value.innerHTML) + 1;
    feedback.innerHTML = "Kliknales na niebieski 1";
    e.stopPropagation();
}
const yellow_prop = (e) => {
    if (parseInt(counter_value.innerHTML) <= 50){
        counter_value.innerHTML = parseInt(counter_value.innerHTML) + 5;
    } else {
        yellow.removeEventListener('click', yellow_prop);
    }
}
const yellow_nprop = (e) => {
    if (parseInt(counter_value.innerHTML) <= 50){
        counter_value.innerHTML = parseInt(counter_value.innerHTML) + 5;
        feedback.innerHTML = "Kliknales na zolty 5";
        e.stopPropagation();
    } else {
        yellow.removeEventListener('click', yellow_nprop);
    }
    
}
const red_prop = (e) => {
    if (parseInt(counter_value.innerHTML) <= 30){
        counter_value.innerHTML = parseInt(counter_value.innerHTML) + 2;
    } else {
        red.removeEventListener('click', red_prop);
    }
}
const red_nprop = (e) => {
    if (parseInt(counter_value.innerHTML) <= 30){
        counter_value.innerHTML = parseInt(counter_value.innerHTML) + 2;
        feedback.innerHTML = "Kliknales na czerwony 2";
        e.stopPropagation();
    } else {
        red.removeEventListener('click', red_nprop);
    }
    
}

const set_handlers = (propagation) => {
    let current_value = parseInt(counter_value.innerHTML);
    if (propagation){        
        blue.removeEventListener('click', blue_nprop);
        blue.addEventListener('click', blue_prop);

        red.removeEventListener('click', red_nprop);
        if (current_value <= 30){
            red.addEventListener('click', red_prop);
        }
        
        yellow.removeEventListener('click', yellow_nprop);
        if (current_value <= 50){
            yellow.addEventListener('click', yellow_prop);
        }
        
    } else {
        
        blue.removeEventListener('click', blue_prop);
        blue.addEventListener('click', blue_nprop);

        red.removeEventListener('click', red_prop);
        if (current_value <= 30){
            red.addEventListener('click', red_nprop);

        }        
        yellow.removeEventListener('click', yellow_prop);
        if (current_value <= 50){
            yellow.addEventListener('click', yellow_nprop);
        }
        
    }
}

blue.addEventListener('click', blue_prop);
yellow.addEventListener('click', yellow_prop);
red.addEventListener('click', red_prop);


restart_button.addEventListener('click', () => {
    counter_value.innerHTML = 0;
    set_handlers(propagation);
})

control_button.addEventListener('click', () => {
    if (propagation){
        propagation = false;
        control_button.innerHTML = "Start propagation";

    } else {
        propagation = true;
        control_button.innerHTML = "Stop propagation";
    }
    set_handlers(propagation);

})
