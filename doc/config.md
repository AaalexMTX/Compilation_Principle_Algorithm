# VS的文件管理
## 新建文件
1. 右击新建的新项默认在根目录下（此时可以一步到胃）
2. 未设置则
```c++
// .vcxproj 文件中手动配置路径
<ItemGroup>
    <ClCompile Include="./src/LexicalAnalyseFunc.cpp">
      <ExcludedFromBuild>false</ExcludedFromBuild>
      <Filter>src</Filter>
    </ClCompile>
</ItemGroup>

// .vcxproj.filter中配置vs的逻辑展示
<ItemGroup>
    //映射的实际路径
    <ClCompile Include="./src/RPNFunc.cpp">
    //逻辑展示路径
      <Filter>源文件\xx\xx</Filter>
    </ClCompile>
  </ItemGroup>

```