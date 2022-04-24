
const renderPosts = async () => {
    let url = "http://localhost:3000/employees";
    const res = await fetch(url);
    const employees = await res.json();
    console.log(employees);
}

renderPosts();