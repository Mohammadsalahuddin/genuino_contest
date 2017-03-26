var g1;

window.onload = function(){
        var g1 = new JustGage({
          id: "g1",
          value: getRandomInt(0, 100),
          min: 0,
          max: 100,
          title: "Temperature",
          titleFontColor:"white",
          label: "Celsius" 
          
        });
        setInterval(function() {
          //g1.refresh(getRandomInt(50, 100));
          g1.refresh(temp);
        }, 2500);
      };

