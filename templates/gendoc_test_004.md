# aaa

## bbb

```c++: key=val, key2=val
    if (rpos < curr) {
        node_text* text = new node_text(str::take_view(line, str::range(rpos, (curr - rpos))));
        context.append_child(text);
        context.rpos(curr);
    }
```
