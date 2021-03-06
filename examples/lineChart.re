/* ported (and simplified) Reason version of a basic d3 line chart
 * via https://bl.ocks.org/d3noob/402dd382a51a4f6eea487f9a35566de0 */
module S = D3.Selection;
let fmtFloat = D3.Helpers.fmtFloat;

let sampleData = [|1, 5, 6, 9, 10, 1, 3, 7, 6, 4, 5|];
let width = 900.;
let height = 500.;
let margin = D3.Helpers.{t: 20., r: 20., b: 20., l: 20.};
let curve = D3.Curve.catmullRom;


module X = D3.Scale.MakeLinearFloat();
module Y = D3.Scale.MakeLinearFloat();

let _ = X.(
  instance
  |. domain([|0., float_of_int(Array.length(sampleData) - 1)|])
  |. range([|0., width|])
);

let _ = Y.(
  instance
  |. domain([|0., float_of_int(D3.Array.max_(sampleData, ()))|])
  |. range([|height, 0.|])
);

let valueLine =
  D3.Line.make()
  |. D3.Line.x((_, idx, _) => X.call(float_of_int(idx)))
  |. D3.Line.curve(curve)
  |. D3.Line.y((value, _, _) => Y.call(value));

let area =
  D3.Area.make()
  |. D3.Area.x((_, idx, _) => X.call(float_of_int(idx)))
  |. D3.Area.curve(curve)
  |. D3.Area.y1((value, _, _) => Y.call(value))
  |. D3.Area.y0((_, _, _) => height -. margin.t);

let svg = D3.Helpers.makeContainer(".linechart", {width, height, margin});

/* Draw x and y axes on the svg */
svg
|. S.append("g")
|. S.attr("class", "axis xAxis")
|. S.attr("transform", "translate(0," ++ (fmtFloat(height -. margin.t) ++ ")"))
|. S.callAxis(D3.Axis.makeBottom(X.instance));

svg
|. S.append("g")
|. S.attr("class", "axis yAxis")
|. S.callAxis(D3.Axis.makeLeft(Y.instance));

/* Draw a line with the data */
svg
|. S.append("path")
|. S.attr("class", "linePath")
|. S.datum(sampleData)
|. S.attr("d", valueLine);

/* Draw an area fill with the data */
svg
|. S.append("path")
|. S.attr("class", "lineArea")
|. S.datum(sampleData)
|. S.attr("d", area);

/* Draw circles for each data point */
svg
|. S.selectAll("circle")
|. S.data(sampleData)
|. S.enter
|. S.append("circle")
|. S.attr("class", "linePoint")
|. S.attrFn("cx", (_, idx, _) => X.call(idx |> float_of_int))
|. S.attrFn("cy", (value, _, _) => Y.call(value))
|. S.attr("r", 3);
