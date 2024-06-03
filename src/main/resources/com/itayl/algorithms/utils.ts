// 54 isnt real
const verteciesInitials = [0, 3, 7, 11, 16, 21, 27, 33, 38, 43, 47, 51, 54];

// 0,3,7,11,16,21,27,33,38,43,47,51
// 0,1,2,3,4,5,6,7,8,09,10,11,12,13
const hexagonsInitials = [0, 3, 7, 12, 16, 19];

console.clear();

function getRowOffset(dot: number): number {
    let i = 0;
    for (i = 1; i < verteciesInitials.length - 1 && verteciesInitials[i] <= dot; i++);
    return i - 1;
}

function devide(a: number, b: number) {
    return Math.floor(a / b);
}

export function nearVertecies(dot: number): Array<number> {
    const vertexRow = getRowOffset(dot);

    const offset = dot - verteciesInitials[vertexRow];

    const above = Math.max(vertexRow - 1, 0);
    const below = Math.min(vertexRow + 1, verteciesInitials.length - 2);

    function modify(addon: number, axis: number) {
        return Math.min(verteciesInitials[axis + 1] - 1, Math.max(verteciesInitials[axis], verteciesInitials[axis] + offset + addon));
    }

    const isEvan = vertexRow % 2 == 0;
    const dots = new Set<number>();

    const isAbove5 = vertexRow > 5;

    dots.add(modify(0, above));
    dots.add(modify(0 + -(isAbove5 && isEvan), below));
    dots.add(modify(isEvan ? 1 + -isAbove5 : -1 + 2 * +isAbove5, isEvan ? below : above));

    dots.delete(dot);
    const arr = Array.from(dots.values()) as Array<number>;

    return arr;
}

export function dotRowToHexRows(row: number) {
    const devided = devide(row, 2);
    const rows = new Set<number>();
    rows.add(Math.max(0, devided - 1));
    rows.add(Math.min(4, devided));
    let arr = Array.from(rows.values()).sort();

    if (row % 2) arr = arr.sort((a, b) => b - a);
    return arr;
}
export function dotToHexes(dot: number) {
    console.clear();
    const vertexRow = getRowOffset(dot);
    const hexRows = dotRowToHexRows(vertexRow);
    const offset = dot - verteciesInitials[vertexRow];

    const hexes = new Set<number>();
    const isEnd = dot === verteciesInitials[vertexRow + 1] - 1;
    const lonleyRow = [1, 3, 5, 6, 8, 10].includes(vertexRow);
    const isFirst = offset === 0;
    const direction = -1;

    function Modify(rowOffset: number, addon: number) {
        const minHextRow = hexagonsInitials[rowOffset];
        const maxHextRow = hexagonsInitials[rowOffset + 1] - 1;
        const hex = Math.min(Math.max(minHextRow + offset + addon, minHextRow), maxHextRow);
        console.log(`\t--hex = ${hex}`);
        return hex;
    }

    hexes.add(Modify(hexRows[0], 0));
    if (vertexRow) hexes.add(Modify(hexRows[0], direction + +(vertexRow === 11)));
    if (hexRows.length > 1 && !(lonleyRow && (isEnd || isFirst))) {
        hexes.add(Modify(hexRows[1], direction + +isEnd));
    }

    const arr = Array.from(hexes.values()).sort();

    return arr;
}
export function joins() {
    const [AREAS, DOTS] = [19, 54];

    const arr: Array<[number, number]> = [];
    for (let index = 0; index < DOTS; index++) {
        const vertecies = nearVertecies(index);
        const hexagons = dotToHexes(index);

        for (const vertex of vertecies) {
            arr.push([index + AREAS, vertex + AREAS]);
        }
        for (const hexagon of hexagons) {
            arr.push([index + AREAS, hexagon]);
            arr.push([hexagon, index + AREAS]);
        }
    }
    arr.sort((before, current) => {
        let compares = before[0] - current[0];
        compares = compares ? compares : before[1] - current[1];
        return compares;
    });

    return arr;
}
