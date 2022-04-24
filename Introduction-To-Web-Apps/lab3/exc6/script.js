let add_button = document.getElementById('add');
let contact_list = document.getElementById('contact-list');

let phone_number = document.getElementById('phone-number');
let name_data = document.getElementById('name-data');


add_button.addEventListener('click', () => {
    if (name_data.value != '' && phone_number.value != ''){
        const li = document.createElement('li');
    
    const info_div = document.createElement('div');
    info_div.id = 'info';
        
    const h3_1 = document.createElement('h3');
    const h3_2 = document.createElement('h3');

    h3_1.innerHTML = name_data.value;
    h3_2.innerHTML = phone_number.value;


    info_div.appendChild(h3_1);
    info_div.appendChild(h3_2);
  

    const button_div = document.createElement('div');
    button_div.id = 'button-box';
  
    const trash_button = document.createElement('button');
    trash_button.innerHTML = "Trash";
    
    

    button_div.appendChild(trash_button);

    li.appendChild(info_div);
    li.appendChild(button_div);

    trash_button.addEventListener('click', () => {
        contact_list.removeChild(li);            
    })

    contact_list.appendChild(li);
    }
})