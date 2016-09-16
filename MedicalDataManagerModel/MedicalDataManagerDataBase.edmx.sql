
-- --------------------------------------------------
-- Entity Designer DDL Script for SQL Server 2005, 2008, 2012 and Azure
-- --------------------------------------------------
-- Date Created: 09/17/2016 01:19:26
-- Generated from EDMX file: \\mac\home\documents\visual studio 2015\Projects\MedicalDataManager\MedicalDataManagerModel\MedicalDataManagerDataBase.edmx
-- --------------------------------------------------

SET QUOTED_IDENTIFIER OFF;
GO
USE [MedicalDataManager];
GO
IF SCHEMA_ID(N'dbo') IS NULL EXECUTE(N'CREATE SCHEMA [dbo]');
GO

-- --------------------------------------------------
-- Dropping existing FOREIGN KEY constraints
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[FK_UserPerson]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Users] DROP CONSTRAINT [FK_UserPerson];
GO
IF OBJECT_ID(N'[dbo].[FK_ActionToken]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Actions] DROP CONSTRAINT [FK_ActionToken];
GO
IF OBJECT_ID(N'[dbo].[FK_ActionActionType]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Actions] DROP CONSTRAINT [FK_ActionActionType];
GO
IF OBJECT_ID(N'[dbo].[FK_ActionDevice]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Actions] DROP CONSTRAINT [FK_ActionDevice];
GO
IF OBJECT_ID(N'[dbo].[FK_DeviceUsesTokenToken]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[DeviceUsesTokens] DROP CONSTRAINT [FK_DeviceUsesTokenToken];
GO
IF OBJECT_ID(N'[dbo].[FK_DeviceUsesTokenDevice]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[DeviceUsesTokens] DROP CONSTRAINT [FK_DeviceUsesTokenDevice];
GO
IF OBJECT_ID(N'[dbo].[FK_TokenPerson]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Tokens] DROP CONSTRAINT [FK_TokenPerson];
GO

-- --------------------------------------------------
-- Dropping existing tables
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[People]', 'U') IS NOT NULL
    DROP TABLE [dbo].[People];
GO
IF OBJECT_ID(N'[dbo].[Devices]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Devices];
GO
IF OBJECT_ID(N'[dbo].[Tokens]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Tokens];
GO
IF OBJECT_ID(N'[dbo].[DeviceUsesTokens]', 'U') IS NOT NULL
    DROP TABLE [dbo].[DeviceUsesTokens];
GO
IF OBJECT_ID(N'[dbo].[Users]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Users];
GO
IF OBJECT_ID(N'[dbo].[ActionTypes]', 'U') IS NOT NULL
    DROP TABLE [dbo].[ActionTypes];
GO
IF OBJECT_ID(N'[dbo].[Actions]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Actions];
GO

-- --------------------------------------------------
-- Creating all tables
-- --------------------------------------------------

-- Creating table 'People'
CREATE TABLE [dbo].[People] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [Firstname] nvarchar(max)  NOT NULL,
    [Lastname] nvarchar(max)  NOT NULL,
    [PhoneNumber] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Devices'
CREATE TABLE [dbo].[Devices] (
    [Id] uniqueidentifier  NOT NULL
);
GO

-- Creating table 'Tokens'
CREATE TABLE [dbo].[Tokens] (
    [Id] uniqueidentifier  NOT NULL,
    [Person_Id] int  NOT NULL
);
GO

-- Creating table 'DeviceUsesTokens'
CREATE TABLE [dbo].[DeviceUsesTokens] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [ReadOnly] bit  NOT NULL,
    [TokenId] uniqueidentifier  NOT NULL,
    [DeviceId] uniqueidentifier  NOT NULL
);
GO

-- Creating table 'Users'
CREATE TABLE [dbo].[Users] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [UserId] nvarchar(max)  NOT NULL,
    [PassHash] nvarchar(max)  NOT NULL,
    [Person_Id] int  NOT NULL
);
GO

-- Creating table 'ActionTypes'
CREATE TABLE [dbo].[ActionTypes] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [Description] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Actions'
CREATE TABLE [dbo].[Actions] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [TokenId] uniqueidentifier  NOT NULL,
    [ActionTypeId] int  NOT NULL,
    [DeviceId] uniqueidentifier  NOT NULL,
    [TimeStamp] datetime  NOT NULL,
    [Payload] nvarchar(max)  NULL
);
GO

-- --------------------------------------------------
-- Creating all PRIMARY KEY constraints
-- --------------------------------------------------

-- Creating primary key on [Id] in table 'People'
ALTER TABLE [dbo].[People]
ADD CONSTRAINT [PK_People]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Devices'
ALTER TABLE [dbo].[Devices]
ADD CONSTRAINT [PK_Devices]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Tokens'
ALTER TABLE [dbo].[Tokens]
ADD CONSTRAINT [PK_Tokens]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'DeviceUsesTokens'
ALTER TABLE [dbo].[DeviceUsesTokens]
ADD CONSTRAINT [PK_DeviceUsesTokens]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Users'
ALTER TABLE [dbo].[Users]
ADD CONSTRAINT [PK_Users]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'ActionTypes'
ALTER TABLE [dbo].[ActionTypes]
ADD CONSTRAINT [PK_ActionTypes]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [PK_Actions]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- --------------------------------------------------
-- Creating all FOREIGN KEY constraints
-- --------------------------------------------------

-- Creating foreign key on [Person_Id] in table 'Users'
ALTER TABLE [dbo].[Users]
ADD CONSTRAINT [FK_UserPerson]
    FOREIGN KEY ([Person_Id])
    REFERENCES [dbo].[People]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_UserPerson'
CREATE INDEX [IX_FK_UserPerson]
ON [dbo].[Users]
    ([Person_Id]);
GO

-- Creating foreign key on [TokenId] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [FK_ActionToken]
    FOREIGN KEY ([TokenId])
    REFERENCES [dbo].[Tokens]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_ActionToken'
CREATE INDEX [IX_FK_ActionToken]
ON [dbo].[Actions]
    ([TokenId]);
GO

-- Creating foreign key on [ActionTypeId] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [FK_ActionActionType]
    FOREIGN KEY ([ActionTypeId])
    REFERENCES [dbo].[ActionTypes]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_ActionActionType'
CREATE INDEX [IX_FK_ActionActionType]
ON [dbo].[Actions]
    ([ActionTypeId]);
GO

-- Creating foreign key on [DeviceId] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [FK_ActionDevice]
    FOREIGN KEY ([DeviceId])
    REFERENCES [dbo].[Devices]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_ActionDevice'
CREATE INDEX [IX_FK_ActionDevice]
ON [dbo].[Actions]
    ([DeviceId]);
GO

-- Creating foreign key on [TokenId] in table 'DeviceUsesTokens'
ALTER TABLE [dbo].[DeviceUsesTokens]
ADD CONSTRAINT [FK_DeviceUsesTokenToken]
    FOREIGN KEY ([TokenId])
    REFERENCES [dbo].[Tokens]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_DeviceUsesTokenToken'
CREATE INDEX [IX_FK_DeviceUsesTokenToken]
ON [dbo].[DeviceUsesTokens]
    ([TokenId]);
GO

-- Creating foreign key on [DeviceId] in table 'DeviceUsesTokens'
ALTER TABLE [dbo].[DeviceUsesTokens]
ADD CONSTRAINT [FK_DeviceUsesTokenDevice]
    FOREIGN KEY ([DeviceId])
    REFERENCES [dbo].[Devices]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_DeviceUsesTokenDevice'
CREATE INDEX [IX_FK_DeviceUsesTokenDevice]
ON [dbo].[DeviceUsesTokens]
    ([DeviceId]);
GO

-- Creating foreign key on [Person_Id] in table 'Tokens'
ALTER TABLE [dbo].[Tokens]
ADD CONSTRAINT [FK_TokenPerson]
    FOREIGN KEY ([Person_Id])
    REFERENCES [dbo].[People]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_TokenPerson'
CREATE INDEX [IX_FK_TokenPerson]
ON [dbo].[Tokens]
    ([Person_Id]);
GO

-- --------------------------------------------------
-- Script has ended
-- --------------------------------------------------