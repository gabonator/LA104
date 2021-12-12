#include <library.h>

int main(void)
{
    BIOS::DBG::Print(R"(<script src="https://code.highcharts.com/highcharts.js"></script>
    <div id="container" style="width:500px; height:300px; background:#808080;"></div>
    <script>
        Highcharts.chart('container', {
            chart: {
                type: "spline",
                events: {
                    load: function () {
                        addPoint = y => this.series[0].addPoint([(new Date()).getTime(), y]);
                    }
                }
            },
            time: {useUTC: false},
            title: {text: 'Live random data'},
            xAxis: {type: 'datetime', tickPixelInterval: 150},
            yAxis: {title: {text: 'Value'}},
            series: [{
                name: 'Random data',
                data: []
            }]
        });
    </script>)");

    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)
    {
        BIOS::SYS::DelayMs(3000);
        BIOS::DBG::Print(R"(<script>addPoint(%d)</script>)", rand() % 100);
    }
    return 0;
}
