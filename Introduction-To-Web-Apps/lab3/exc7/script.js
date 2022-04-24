let citiesFromMalopolska = document.getElementById('citiesFromMalopolska');
let doubleACities = document.getElementById('doubleACities');
let fifthCity = document.getElementById('fifthCity');

const renderPosts = async () => {
    let url = "http://localhost:3000/cities";

    const res = await fetch(url);
    const cities = await res.json();
    a(cities);
    b(cities);
    c(cities);
}

const a = (cities) => {
    cities.forEach(city => {
        if (city.province == 'małopolskie'){
            if (citiesFromMalopolska.innerHTML == ""){
                citiesFromMalopolska.innerHTML += city.name;    
            } else {
                citiesFromMalopolska.innerHTML += ", " + city.name;    
            }
        }
    });
}
const b = (cities) => {
    cities.forEach(city => {
        let counter = 0;
        for (let letter of city.name){
            if (letter == "a") counter++;
        }
        if (counter == 2){
            if (doubleACities.innerHTML == ""){
                doubleACities.innerHTML += city.name;    
            } else {
                doubleACities.innerHTML += ", " + city.name;    
            }
        }
        
    });
}
const c = (cities) => {
    let array = [];
    cities.forEach((city) => {
        array.push([city.name, parseInt(city.dentensity)]);
    })
    array.sort((a, b) => b[1] - a[1]);
    fifthCity.innerHTML = array[5][0];
}

const e = (cities) => {
    let cities_below = 0;
    let cities_above = 0

    cities.forEach((city) => {
        if (parseInt(city.people) > 80000){
            cities_above++;
        } else {
            cities_below++;
        }
    });
    console.log("Cities below: ", cities_below);
    console.log("Cities above: ", cities_above);

}
const f = (cities) => {
    let totalArea = 0;
    let numberNumberOfCities = 0;
    cities.forEach((city) => {
        if (city.township[0] == 'P'){
            totalArea += parseFloat(city.area);
            numberNumberOfCities ++;
        }
    });
    console.log("Average area: ", totalArea / numberNumberOfCities);
}

renderPosts();