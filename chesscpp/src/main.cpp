float big_loop(int size) {
    int result = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            result += 1;
        }
    }
    return (float) result;
}