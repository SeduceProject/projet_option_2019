$(document).ready(function() {
    $("#successAlert").hide();
    $("#errorAlert").hide();

    
    $("#uploadBtn").click(function(){
        $("#successAlert").hide();
        $("#errorAlert").hide();
        $(this).attr("disabled", true);
        $(this).html('Uploading...')
        sendTemplate();});
        
});

function sendTemplate(){
    $.ajax({
        url: '/upload',
        dataType: 'json',
        type: 'post',
        contentType: 'application/json',
        data: JSON.stringify( { "template": editor.getValue() } ),
        processData: false,
        success: function(result,status,xhr){
            successCallback(result);
        },
        error: function(xhr, errorThrown){
            $("#errorAlert").text(xhr.status + " error.");
            $("#errorAlert").show();
            console.log(errorThrown);
        },
        complete : function(){
            $("#uploadBtn").attr("disabled", false);
            $("#uploadBtn").text("Upload code");
        }
    });
}

function successCallback(result,status,xhr){
    let wrong = [];
    $.each(result, function(key, value){
        if(value == 0){
            wrong.push(key);
        }
    });
    if(Array.isArray(wrong) && !wrong.length){
        $("#successAlert").show();
    }
    else{
        $("#errorAlert").text("Couldn't upload for cards : " + wrong.join(", ")) + ".";
        $("#errorAlert").show();
    }
}