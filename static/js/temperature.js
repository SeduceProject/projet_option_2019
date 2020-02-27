var card;
var sensor;
$(document).ready(function() {
    $("#cardList").change(function(){
        updateCard();});
    $("#sensorList").change(function(){
        updateSensor();});

    updateCard();
    updateSensor();
    updateTemperature();
});


function updateCard(){
    card = $("#cardList option:selected").text();
    console.log(card + " selected");
}
function updateSensor(){
    sensor = $("#sensorList option:selected").text();
    console.log(sensor + " selected");
}
function updateTemperature(){
    $.ajax({
        url: '/get_temperature',
        dataType: 'json',
        type: 'post',
        contentType: 'application/json',
        data: JSON.stringify({"card": card, "sensor": sensor}),
        processData: false,
        success: function(result,status,xhr){
            let temp = result["temp"];
            if(parseInt(temp) <= -127){
                displayTemperature("-");}
            else{
                displayTemperature(temp)}
        },
        error: function(xhr, errorThrown){
            displayTemperature("-")
        }
    });
}

function displayTemperature(temp){
    console.log("temp = " +temp)
    $("#temp").text(temp);
    setTimeout(updateTemperature, 1000)
}