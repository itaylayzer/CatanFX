import { expect, test } from "bun:test";
import { dotRowToHexRows, dotToHexes, joins, nearVertecies } from "./utils";

test("edges:", () => {
    expect(nearVertecies(12).sort()).toEqual([7, 17, 8].sort()); // ; even:true  ; r:3 o:1
    expect(nearVertecies(17).sort()).toEqual([12, 22, 23].sort()); // ; even:true  ; r:4 o:1
    expect(nearVertecies(22).sort()).toEqual([16, 17, 28].sort()); // ; even:false ; r:5 o:1
    expect(nearVertecies(28).sort()).toEqual([22, 33, 34].sort()); // ; even:true  ; r:6 o:2
    expect(nearVertecies(34).sort()).toEqual([28, 29, 39].sort()); // ; even:false ; r:7 o:2
    expect(nearVertecies(39).sort()).toEqual([44, 43, 34].sort()); // ; even:true  ; r:8 o:1
    expect(nearVertecies(44).sort()).toEqual([39, 40, 48].sort()); // ; even:false ; r:9 o:1
    expect(nearVertecies(48).sort()).toEqual([51, 52, 44].sort()); // ; even:true  ; r:10 o:1
    expect(nearVertecies(51).sort()).toEqual([47, 48].sort()); // ; even:false ; r:11 o:1
});
test("hexes rows:", () => {
    expect(dotRowToHexRows(2)).toEqual([0, 1]);
    expect(dotRowToHexRows(3)).toEqual([1, 0]);
});
test("hexes:", () => {
    expect(dotToHexes(0).sort()).toEqual([0].sort());
    expect(dotToHexes(1).sort()).toEqual([1].sort());
    expect(dotToHexes(2).sort()).toEqual([2].sort());
    expect(dotToHexes(4).sort()).toEqual([0, 1].sort());
    expect(dotToHexes(6).sort()).toEqual([2].sort());
    expect(dotToHexes(7).sort()).toEqual([0, 3].sort());
    expect(dotToHexes(10).sort()).toEqual([2, 6].sort());
    expect(dotToHexes(12).sort()).toEqual([0, 3, 4].sort());
    expect(dotToHexes(15).sort()).toEqual([6].sort());
    expect(dotToHexes(20).sort()).toEqual([6, 11].sort());
    expect(dotToHexes(21).sort()).toEqual([7].sort());
    expect(dotToHexes(22).sort()).toEqual([7, 8, 3].sort());
    expect(dotToHexes(26).sort()).toEqual([11].sort());
    expect(dotToHexes(27).sort()).toEqual([7].sort());
    expect(dotToHexes(28).sort()).toEqual([7, 8, 12].sort());
    expect(dotToHexes(32).sort()).toEqual([11].sort());
    expect(dotToHexes(33).sort()).toEqual([7, 12].sort());
    expect(dotToHexes(37).sort()).toEqual([15, 11].sort());
    expect(dotToHexes(39).sort()).toEqual([12, 13, 16].sort());
    expect(dotToHexes(41).sort()).toEqual([14, 15, 18].sort());
    expect(dotToHexes(42).sort()).toEqual([15].sort());
    expect(dotToHexes(51).sort()).toEqual([16].sort());
    expect(dotToHexes(52).sort()).toEqual([17].sort());
    expect(dotToHexes(53).sort()).toEqual([18].sort());
});
test("join", () => {
    const arcs = joins();
    console.clear();
    const problems: Array<number> = [];
    for (let index = 0; index < 19; index++) {
        if (arcs.filter((v) => v[0] === index).length !== 6) problems.push(index);
    }
    console.log(problems);
    expect(problems.length).toBe(0);
});
