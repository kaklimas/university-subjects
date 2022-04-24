let img = document.getElementById('img');
let imgBox = document.getElementById('img-box');
let condition = true;
let currentSize = document.getElementById('size');
let info = document.getElementById('info');
let displayingInfo = false;
let container = document.getElementsByClassName('container')[0];
let previous_key = null;

console.log(container.style);
document.addEventListener('keydown', (e)=>{
    e.preventDefault();
})

img.addEventListener('click', (e) => {
    console.log(previous_key);
    if (previous_key == "Control"){
        if (!displayingInfo){
            info.style = "display: flex;"
            container.style = "background-color: rgb(170, 164, 164);"
            displayingInfo = true;
            previous_key = null;
        }
        e.stopPropagation();
    }
});
document.addEventListener('keydown', (e) => {
    previous_key = e.key;
})

document.addEventListener('click', (e) => {
    if (displayingInfo){
        info.style = "display: none;"
        container.style = "background-color: white;"
        displayingInfo = false;
    }
    e.stopPropagation();
})

const changeSize = (e) => {
    let imgSize = parseFloat(window.getComputedStyle(imgBox, null).getPropertyValue('font-size'));
    let newImgSize = null;
    switch (e.key){        
        case "ArrowUp":    
            newImgSize = imgSize + Math.round(imgSize/10);
            if (newImgSize < 330 && condition){
                imgBox.style = "font-size: " + newImgSize + "px";
                currentSize.innerHTML = newImgSize; 
            } else if (newImgSize >= 330){
                condition = false;
                img.className = "em em-boom";
                currentSize.innerHTML = newImgSize; 
            }
            
            break;
        case "ArrowDown":
            newImgSize = imgSize - imgSize/10;
            if (newImgSize > 5 && condition){
                currentSize.innerHTML = newImgSize;
                imgBox.style = "font-size: " + newImgSize + "px";
            }
            break;
    }
}

document.addEventListener('keydown', (e) => changeSize(e));