from collections.abc import Collection, Iterator
from typing import TypeVar

T = TypeVar('T')


class BinarySearchTree(Collection[T]):
    def __init__(self) -> None:
        ...

    def __len__(self) -> int:
        ...

    def __iter__(self) -> Iterator[T]:
        ...

    def __contains__(self, value: T) -> bool:
        ...

    def add(self, value: T) -> None:
        ...
