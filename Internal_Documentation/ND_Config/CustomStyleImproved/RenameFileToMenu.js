let called = false;

function func()
{
    //clearInterval(myInterval);
    var elems = window.document.getElementsByClassName("MTabTitle");

    if(elems == null)
        return;

    for(var i = 0; i < elems.length; i++)
    {
        if(elems[i].innerHTML === "Files")
        {
            elems[i].innerHTML = "Menu";
        }
    }
}

if(called === false)
{
    myInterval = setInterval(func, 2000);
    called = true;
}