import { joins } from "./utils";

const message = joins()
    .map((val) => `${val[0]}:${val[1]}`)
    .join("\n");
console.clear();
console.log(message);
