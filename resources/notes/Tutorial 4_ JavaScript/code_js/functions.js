function calculateSquareRoot() {
    // Get the value of the input field with id="number"
    var number = document.getElementById("number").value;
    
    // Calculate the square root of the number
    var squareRoot = Math.sqrt(number);
    
    // Display the result in the element with id="result"
    document.getElementById("result").innerHTML = 
    "The square root of " + number + " is " + squareRoot;
}

function name(x){
    return x*x;
}