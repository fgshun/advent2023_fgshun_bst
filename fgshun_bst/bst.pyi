from typing import Generic, TypeVar

T = TypeVar('T')


class BinarySearchTree(Generic[T]):
    def __init__(self) -> None:
        ...

    def __contains__(self, value: T) -> bool:
        ...

    def add(self, value: T) -> None:
        ...
