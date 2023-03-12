# 简介

[VsCodeVim](https://github.com/VSCodeVim) 支持输入法的自动切换, 它通过一个外部的[输入法切换工具来实现](https://github.com/VSCodeVim/Vim/blob/f3f9850739e93fe5cc95827a64180fbf67fd377d/README.md#input-method), 目前这个工具推荐的是 [im-select](https://github.com/daipeihust/im-select).

由于从 Windows 10 开始, 系统默认不再安装英文键盘, 而是由输入法来提供英文输入, 而 im-select windows 版需要英文键盘, 所以在 Windows 10 上使用 VsCodeVim 时, 无法通过 im-select 来切换输入法, 也就无法通过 VsCodeVim 来切换输入法.

im-select-mspy 是一个针对微软拼音输入法的输入法切换工具, 它可以在不安装英文键盘的情况下, 完成 VsCodeVim 所需要的输入法信息获取和切换.

