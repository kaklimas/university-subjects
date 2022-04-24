let mountains_url = "https://cdn.britannica.com/67/19367-050-885866B4/Valley-Taurus-Mountains-Turkey.jpg";
let sea_url = "https://d2v9ipibika81v.cloudfront.net/uploads/sites/72/fantastic-seascape-with-ripples-1140x684.jpg";
let photo_type = 1;


const change = () => {
    
    if (photo_type == 1){
        document.getElementById("img").src = sea_url;
        document.getElementById("img").style="border: 5px solid blue";
        photo_type = 2;
    } else {
        document.getElementById("img").src = mountains_url;
        document.getElementById("img").style="border: 5px solid red";
        photo_type = 1;
    }
}